// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#include "version.h"

#include <boost/format.hpp>
#include <cryptonotecore/Core.h>
#include <cryptonotecore/CryptoNoteFormatUtils.h>
#include <cryptonotecore/Currency.h>
#include <cryptonoteprotocol/CryptoNoteProtocolHandler.h>
#include <ctime>
#include <daemon/DaemonCommandsHandler.h>
#include <p2p/NetNode.h>
#include <rpc/JsonRpc.h>
#include <serialization/SerializationTools.h>
#include <utilities/ColouredMsg.h>
#include <utilities/FormatTools.h>
#include <utilities/Utilities.h>

namespace
{
    template<typename T> static bool print_as_json(const T &obj)
    {
        std::cout << CryptoNote::storeToJson(obj) << ENDL;
        return true;
    }

    std::string printTransactionShortInfo(const CryptoNote::CachedTransaction &transaction)
    {
        std::stringstream ss;

        ss << "id: " << transaction.getTransactionHash() << std::endl;
        ss << "fee: " << transaction.getTransactionFee() << std::endl;
        ss << "blobSize: " << transaction.getTransactionBinaryArray().size() << std::endl;

        return ss.str();
    }

    std::string printTransactionFullInfo(const CryptoNote::CachedTransaction &transaction)
    {
        std::stringstream ss;
        ss << printTransactionShortInfo(transaction);
        ss << "JSON: \n" << CryptoNote::storeToJson(transaction.getTransaction()) << std::endl;

        return ss.str();
    }

} // namespace

DaemonCommandsHandler::DaemonCommandsHandler(
    CryptoNote::Core &core,
    CryptoNote::NodeServer &srv,
    std::shared_ptr<Logging::LoggerManager> log,
    CryptoNote::RpcServer *prpc_server):
    m_core(core),
    m_srv(srv),
    logger(log, "daemon"),
    m_logManager(log),
    m_prpc_server(prpc_server)
{
    m_consoleHandler.setHandler(
        "?",
        std::bind(&DaemonCommandsHandler::help, this, std::placeholders::_1),
        "Show this help");
    m_consoleHandler.setHandler(
        "exit",
        std::bind(&DaemonCommandsHandler::exit, this, std::placeholders::_1),
        "Shutdown the daemon");
    m_consoleHandler.setHandler(
        "help",
        std::bind(&DaemonCommandsHandler::help, this, std::placeholders::_1),
        "Show this help");
    m_consoleHandler.setHandler(
        "print_pl",
        std::bind(&DaemonCommandsHandler::print_pl, this, std::placeholders::_1),
        "Print peer list");
    m_consoleHandler.setHandler(
        "print_cn",
        std::bind(&DaemonCommandsHandler::print_cn, this, std::placeholders::_1),
        "Print connections");
    m_consoleHandler.setHandler(
        "print_bc",
        std::bind(&DaemonCommandsHandler::print_bc, this, std::placeholders::_1),
        "Print blockchain info in a given blocks range, print_bc <begin_height> [<end_height>]");
    m_consoleHandler.setHandler(
        "print_block",
        std::bind(&DaemonCommandsHandler::print_block, this, std::placeholders::_1),
        "Print block, print_block <block_hash> | <block_height>");
    m_consoleHandler.setHandler(
        "print_tx",
        std::bind(&DaemonCommandsHandler::print_tx, this, std::placeholders::_1),
        "Print transaction, print_tx <transaction_hash>");
    m_consoleHandler.setHandler(
        "print_pool",
        std::bind(&DaemonCommandsHandler::print_pool, this, std::placeholders::_1),
        "Print transaction pool (long format)");
    m_consoleHandler.setHandler(
        "print_pool_sh",
        std::bind(&DaemonCommandsHandler::print_pool_sh, this, std::placeholders::_1),
        "Print transaction pool (short format)");
    m_consoleHandler.setHandler(
        "set_log",
        std::bind(&DaemonCommandsHandler::set_log, this, std::placeholders::_1),
        "set_log <level> - Change current log level, <level> is a number 0-4");
    m_consoleHandler.setHandler(
        "status",
        std::bind(&DaemonCommandsHandler::status, this, std::placeholders::_1),
        "Show daemon status");
}

//--------------------------------------------------------------------------------
std::string DaemonCommandsHandler::get_commands_str()
{
    std::stringstream ss;
    ss << CryptoNote::CRYPTONOTE_NAME << " v" << PROJECT_VERSION_LONG << ENDL;
    ss << "Commands: " << ENDL;
    std::string usage = m_consoleHandler.getUsage();
    boost::replace_all(usage, "\n", "\n  ");
    usage.insert(0, "  ");
    ss << usage << ENDL;
    return ss.str();
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::exit(const std::vector<std::string> &args)
{
    std::cout << InformationMsg("================= EXITING ==================\n"
                                "== PLEASE WAIT, THIS MAY TAKE A LONG TIME ==\n"
                                "============================================\n");

    /* Set log to max when exiting. Sometimes this takes a while, and it helps
       to let users know the daemon is still doing stuff */
    m_logManager->setMaxLevel(Logging::TRACE);
    m_consoleHandler.requestStop();
    m_srv.sendStopSignal();
    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::help(const std::vector<std::string> &args)
{
    std::cout << get_commands_str() << ENDL;
    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_pl(const std::vector<std::string> &args)
{
    m_srv.log_peerlist();
    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_cn(const std::vector<std::string> &args)
{
    m_srv.get_payload_object().log_connections();
    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_bc(const std::vector<std::string> &args)
{
    if (!args.size())
    {
        std::cout << "need block index parameter" << ENDL;
        return false;
    }

    uint32_t start_index = 0;
    uint32_t end_index = 0;
    uint32_t end_block_parametr = m_core.getTopBlockIndex();

    if (!Common::fromString(args[0], start_index))
    {
        std::cout << "wrong starter block index parameter" << ENDL;
        return false;
    }

    if (args.size() > 1 && !Common::fromString(args[1], end_index))
    {
        std::cout << "wrong end block index parameter" << ENDL;
        return false;
    }

    if (end_index == 0)
    {
        end_index = start_index;
    }

    if (end_index > end_block_parametr)
    {
        std::cout << "end block index parameter shouldn't be greater than " << end_block_parametr << ENDL;
        return false;
    }

    if (end_index < start_index)
    {
        std::cout << "end block index should be greater than or equal to starter block index" << ENDL;
        return false;
    }

    CryptoNote::COMMAND_RPC_GET_BLOCK_HEADERS_RANGE::request req;
    CryptoNote::COMMAND_RPC_GET_BLOCK_HEADERS_RANGE::response res;
    CryptoNote::JsonRpc::JsonRpcError error_resp;

    req.start_height = start_index;
    req.end_height = end_index;

    // TODO: implement m_is_rpc handling like in monero?
    if (!m_prpc_server->on_get_block_headers_range(req, res, error_resp) || res.status != CORE_RPC_STATUS_OK)
    {
        // TODO res.status handling
        std::cout << "Response status not CORE_RPC_STATUS_OK" << ENDL;
        return false;
    }

    const CryptoNote::Currency &currency = m_core.getCurrency();

    bool first = true;
    for (CryptoNote::block_header_response &header : res.headers)
    {
        if (!first)
        {
            std::cout << ENDL;
            first = false;
        }

        std::cout << "height: " << header.height << ", timestamp: " << header.timestamp
                  << ", difficulty: " << header.difficulty << ", size: " << header.block_size
                  << ", transactions: " << header.num_txes << ENDL
                  << "major version: " << unsigned(header.major_version)
                  << ", minor version: " << unsigned(header.minor_version) << ENDL << "block id: " << header.hash
                  << ", previous block id: " << header.prev_hash << ENDL << "difficulty: " << header.difficulty
                  << ", nonce: " << header.nonce << ", reward: " << currency.formatAmount(header.reward) << ENDL;
    }

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::set_log(const std::vector<std::string> &args)
{
    if (args.size() != 1)
    {
        std::cout << "use: set_log <log_level_number_0-4>" << ENDL;
        return true;
    }

    uint16_t l = 0;
    if (!Common::fromString(args[0], l))
    {
        std::cout << "wrong number format, use: set_log <log_level_number_0-4>" << ENDL;
        return true;
    }

    ++l;

    if (l > Logging::TRACE)
    {
        std::cout << "wrong number range, use: set_log <log_level_number_0-4>" << ENDL;
        return true;
    }

    m_logManager->setMaxLevel(static_cast<Logging::Level>(l));
    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_block_by_height(uint32_t height)
{
    if (height - 1 > m_core.getTopBlockIndex())
    {
        std::cout << "block wasn't found. Current block chain height: " << m_core.getTopBlockIndex() + 1
                  << ", requested: " << height << std::endl;
        return false;
    }

    auto hash = m_core.getBlockHashByIndex(height - 1);
    std::cout << "block_id: " << hash << ENDL;
    print_as_json(m_core.getBlockByIndex(height - 1));

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_block_by_hash(const std::string &arg)
{
    Crypto::Hash block_hash;
    if (!parse_hash256(arg, block_hash))
    {
        return false;
    }

    if (m_core.hasBlock(block_hash))
    {
        print_as_json(m_core.getBlockByHash(block_hash));
    }
    else
    {
        std::cout << "block wasn't found: " << arg << std::endl;
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_block(const std::vector<std::string> &args)
{
    if (args.empty())
    {
        std::cout << "expected: print_block (<block_hash> | <block_height>)" << std::endl;
        return true;
    }

    const std::string &arg = args.front();
    try
    {
        uint32_t height = boost::lexical_cast<uint32_t>(arg);
        print_block_by_height(height);
    }
    catch (boost::bad_lexical_cast &)
    {
        print_block_by_hash(arg);
    }

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_tx(const std::vector<std::string> &args)
{
    if (args.empty())
    {
        std::cout << "expected: print_tx <transaction hash>" << std::endl;
        return true;
    }

    const std::string &str_hash = args.front();
    Crypto::Hash tx_hash;
    if (!parse_hash256(str_hash, tx_hash))
    {
        return true;
    }

    std::vector<Crypto::Hash> tx_ids;
    tx_ids.push_back(tx_hash);
    std::vector<CryptoNote::BinaryArray> txs;
    std::vector<Crypto::Hash> missed_ids;
    m_core.getTransactions(tx_ids, txs, missed_ids);

    if (1 == txs.size())
    {
        CryptoNote::CachedTransaction tx(txs.front());
        print_as_json(tx.getTransaction());
    }
    else
    {
        std::cout << "transaction wasn't found: <" << str_hash << '>' << std::endl;
    }

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_pool(const std::vector<std::string> &args)
{
    std::cout << "Pool state: \n";
    auto pool = m_core.getPoolTransactions();

    for (const auto &tx : pool)
    {
        CryptoNote::CachedTransaction ctx(tx);
        std::cout << printTransactionFullInfo(ctx) << "\n";
    }

    std::cout << std::endl;

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::print_pool_sh(const std::vector<std::string> &args)
{
    const auto pool = m_core.getPoolTransactions();

    if (pool.size() == 0)
    {
        std::cout << InformationMsg("\nPool state: ") << SuccessMsg("Empty.") << std::endl;
        return true;
    }

    std::cout << InformationMsg("\nPool state:\n");

    uint64_t totalSize = 0;

    const float maxTxSize = Utilities::getMaxTxSize(m_core.getTopBlockIndex());

    for (const auto &tx : pool)
    {
        CryptoNote::CachedTransaction ctx(tx);

        std::cout << InformationMsg("Hash: ") << SuccessMsg(ctx.getTransactionHash())
                  << InformationMsg(", Fusion: ");

        if (ctx.getTransactionFee() == 0)
        {
            std::cout << SuccessMsg("Yes") << std::endl;
        }
        else
        {
            std::cout << WarningMsg("No") << std::endl;
        }

        totalSize += ctx.getTransactionBinaryArray().size();
    }

    const float blocksRequiredToClear = std::ceil(totalSize / maxTxSize);

    std::cout << InformationMsg("\nTotal transactions: ") << SuccessMsg(pool.size())
              << InformationMsg("\nTotal size of transactions: ") << SuccessMsg(Utilities::prettyPrintBytes(totalSize))
              << InformationMsg("\nEstimated full blocks to clear: ") << SuccessMsg(blocksRequiredToClear) << std::endl << std::endl;

    return true;
}

//--------------------------------------------------------------------------------
bool DaemonCommandsHandler::status(const std::vector<std::string> &args)
{
    CryptoNote::COMMAND_RPC_GET_INFO::request ireq;
    CryptoNote::COMMAND_RPC_GET_INFO::response iresp;

    if (!m_prpc_server->on_get_info(ireq, iresp) || iresp.status != CORE_RPC_STATUS_OK)
    {
        std::cout << WarningMsg("Problem retrieving information from RPC server.") << std::endl;
        return false;
    }

    const std::time_t uptime = std::time(nullptr) - iresp.start_time;

    const uint64_t seconds = uptime;
    const uint64_t minutes = seconds / 60;
    const uint64_t hours = minutes / 60;
    const uint64_t days = hours / 24;

    const std::string uptimeStr = std::to_string(days) + "d "
                                + std::to_string(hours % 24) + "h "
                                + std::to_string(minutes % 60) + "m "
                                + std::to_string(seconds % 60) + "s";

    const auto forkStatus = Utilities::get_fork_status(iresp.network_height, iresp.upgrade_heights, iresp.supported_height);

    std::vector<std::tuple<std::string, std::string>> statusTable;

    statusTable.push_back({"Local Height", std::to_string(iresp.height)});
    statusTable.push_back({"Network Height", std::to_string(iresp.network_height)});
    statusTable.push_back({"Percentage Synced", Utilities::get_sync_percentage(iresp.height, iresp.network_height) + "%"});
    statusTable.push_back({"Network Hashrate", Utilities::get_mining_speed(iresp.hashrate)});
    statusTable.push_back({"Block Version", "v" + std::to_string(iresp.major_version)});
    statusTable.push_back({"Incoming Connections", std::to_string(iresp.incoming_connections_count)});
    statusTable.push_back({"Outgoing Connections", std::to_string(iresp.outgoing_connections_count)});
    statusTable.push_back({"Uptime", uptimeStr});
    statusTable.push_back({"Fork Status", Utilities::get_update_status(forkStatus)});
    statusTable.push_back({"Next Fork", Utilities::get_fork_time(iresp.network_height, iresp.upgrade_heights)});
    statusTable.push_back({"Transaction Pool Size", std::to_string(m_core.getPoolTransactionHashes().size())});
    statusTable.push_back({"Version", PROJECT_VERSION});

    size_t longestValue = 0;
    size_t longestDescription = 0;

    /* Figure out the dimensions of the table */
    for (const auto [value, description] : statusTable)
    {
        if (value.length() > longestValue)
        {
            longestValue = value.length();
        }

        if (description.length() > longestDescription)
        {
            longestDescription = description.length();
        }
    }

    /* Need 7 extra chars for all the padding and borders in addition to the
     * values inside the table */
    const size_t totalTableWidth = longestValue + longestDescription + 7;

    /* Table border */
    std::cout << std::string(totalTableWidth, '-') << std::endl;

    /* Output the table itself */
    for (const auto [value, description] : statusTable)
    {
        std::cout << "| " << InformationMsg(value, longestValue) << " ";
        std::cout << "| " << SuccessMsg(description, longestDescription) << " |" << std::endl;
    }

    /* Table border */
    std::cout << std::string(totalTableWidth, '-') << std::endl;

    if (forkStatus == Utilities::OutOfDate)
    {
        std::cout << WarningMsg(Utilities::get_upgrade_info(iresp.supported_height, iresp.upgrade_heights)) << std::endl;
    }

    return true;
}
