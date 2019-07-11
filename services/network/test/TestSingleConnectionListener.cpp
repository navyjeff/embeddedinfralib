#include "gmock/gmock.h"
#include "infra/util/test_helper/MockCallback.hpp"
#include "infra/util/test_helper/MockHelpers.hpp"
#include "services/network/SingleConnectionListener.hpp"
#include "services/network/test_doubles/ConnectionMock.hpp"

class SingleConnectionListenerTest
    : public testing::Test
{
public:
    class ConnectionObserverMock
        : public services::ConnectionObserverMock
    {
    public:
        MOCK_METHOD0(Constructed, void());
        MOCK_METHOD0(Destructed, void());
    };

    testing::StrictMock<ConnectionObserverMock> connectionObserverMock;

    class ConnectionObserverStorage
        : public services::ConnectionObserver
    {
    public:
        ConnectionObserverStorage(ConnectionObserverMock& base)
            : base(base)
        {
            base.Constructed();
        }

        ~ConnectionObserverStorage()
        {
            base.Destructed();
        }

        virtual void SendStreamAvailable(infra::SharedPtr<infra::StreamWriter>&& stream) override
        {
            base.SendStreamAvailable(std::move(stream));
        }

        virtual void DataReceived() override
        {
            base.DataReceived();
        }

    private:
        ConnectionObserverMock& base;
    };

    infra::Creator<services::ConnectionObserver, ConnectionObserverStorage, void()> connectionObserverCreator
    { [this](infra::Optional<ConnectionObserverStorage>& connectionObserver)
        { connectionObserver.Emplace(connectionObserverMock); }
    };

    testing::StrictMock<services::ConnectionFactoryMock> connectionFactory;
    services::ServerConnectionObserverFactory* serverConnectionObserverFactory;
    infra::Execute execute{ [this]()
    {
        EXPECT_CALL(connectionFactory, Listen(1, testing::_, services::IPVersions::both)).WillOnce(testing::DoAll(infra::SaveRef<1>(&serverConnectionObserverFactory), testing::Return(nullptr)));
    } };
    services::SingleConnectionListener listener{ connectionFactory, 1, { connectionObserverCreator } };
    infra::MockCallback<void(infra::SharedPtr<services::ConnectionObserver>)> connectionAccepted;
    infra::SharedPtr<services::ConnectionObserver> connectionObserver;
    testing::StrictMock<services::ConnectionMock> connection;
};

TEST_F(SingleConnectionListenerTest, create_connection)
{
    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
        {
            connectionObserver->Attach(connection);
            connectionAccepted.callback(connectionObserver);
        }
        , services::IPAddress{});

    EXPECT_CALL(connectionObserverMock, Destructed());
    connectionObserver = nullptr;
}

TEST_F(SingleConnectionListenerTest, second_connection_cancels_first)
{
    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
        {
            connectionObserver->Attach(connection);
            connectionAccepted.callback(connectionObserver);
        }
        , services::IPAddress{});

    EXPECT_CALL(connection, AbortAndDestroy());
    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
        {
            connectionObserver->Attach(connection);
            connectionAccepted.callback(connectionObserver);
        }
        , services::IPAddress{});

    EXPECT_CALL(connectionObserverMock, Destructed());
    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    connectionObserver = nullptr;

    EXPECT_CALL(connectionObserverMock, Destructed());
    connectionObserver = nullptr;
}

TEST_F(SingleConnectionListenerTest, second_connection_while_first_is_destroyed_but_not_allocatable)
{
    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
    {
        connectionObserver->Attach(connection);
        connectionAccepted.callback(connectionObserver);
    }
    , services::IPAddress{});

    infra::WeakPtr<services::ConnectionObserver> weakObserver = connectionObserver;
    EXPECT_CALL(connectionObserverMock, Destructed());
    connection.ResetOwnership();
    connectionObserver = nullptr;

    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
    {
        connectionObserver->Attach(connection);
        connectionAccepted.callback(connectionObserver);
    }
    , services::IPAddress{});

    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    weakObserver = nullptr;

    EXPECT_CALL(connectionObserverMock, Destructed());
    connectionObserver = nullptr;
}

TEST_F(SingleConnectionListenerTest, third_connection_while_second_has_not_yet_been_made)
{
    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
    {
        connectionObserver->Attach(connection);
        connectionAccepted.callback(connectionObserver);
    }
    , services::IPAddress{});

    infra::WeakPtr<services::ConnectionObserver> weakObserver = connectionObserver;
    EXPECT_CALL(connectionObserverMock, Destructed());
    connection.ResetOwnership();
    connectionObserver = nullptr;

    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
    {
        EXPECT_EQ(nullptr, connectionObserver);
    }
    , services::IPAddress{});

    serverConnectionObserverFactory->ConnectionAccepted(
        [this](infra::SharedPtr<services::ConnectionObserver> connectionObserver)
    {
        connectionObserver->Attach(connection);
        connectionAccepted.callback(connectionObserver);
    }
    , services::IPAddress{});

    EXPECT_CALL(connectionObserverMock, Constructed());
    EXPECT_CALL(connectionAccepted, callback(testing::_)).WillOnce(testing::SaveArg<0>(&connectionObserver));
    weakObserver = nullptr;

    EXPECT_CALL(connectionObserverMock, Destructed());
    connectionObserver = nullptr;
}
