
#include <stdlib.h>
#include <iostream>
#include "customer_service.grpc.pb.h"

using customer_service::Customer;
using customer_service::Customers;

#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define COLUMN_PLACEHOLDER "<COLUMN>"
#define GET_CUSTOMER_BY_ID "select * from customer where id = ?"
#define GET_CUSTOMERS_BY_CRITERIA "select * from customer where " COLUMN_PLACEHOLDER " = ?"

using namespace std;

class CustomerDataSource {

private:

    string db_server;
    string db_user;
    string db_password;
    string db;

    string method;

	sql::Driver* driver;
    sql::Connection* connection;

public:

    CustomerDataSource(){

		driver = sql::mysql::get_driver_instance();
        db_server = getenv("DB_SERVER");
        db_user = getenv("DB_USER");
        db_password = getenv("DB_PASSWORD");
        db = getenv("DB_NAME");

        sql::Connection* connectionInit(driver->connect(db_server, db_user, db_password));
        connection = connectionInit;
	}

    ~CustomerDataSource(){
        delete connection;
    }

    Customer GetCustomerById (int id) {

        method = "GetCustomerById(" + to_string(id) + ")";

		Customer customer;

		try {

            reconnectWhenNeeded();
            connection->setSchema(db);

			sql::PreparedStatement* prepared_statement = connection->prepareStatement(GET_CUSTOMER_BY_ID);
			prepared_statement->setInt(1, id);
			sql::ResultSet* result(prepared_statement->executeQuery());

			while (result->next()) {
                fillCustomer(result, &customer);
			}

			delete prepared_statement;
			delete result;

		} catch (sql::SQLException& e) {
            handleSQLException(e);
		}

		return customer;
	}


    Customers GetCustomers(const string criteria, const string value){

        method = "GetCustomerById(" + criteria + ", " + value + ")";

        Customers customers;

        try {

            reconnectWhenNeeded();
            connection->setSchema(db);

            string sqlWithCriteria = GET_CUSTOMERS_BY_CRITERIA;
            replace(sqlWithCriteria, COLUMN_PLACEHOLDER, criteria);
            sql::PreparedStatement* prepared_statement = connection->prepareStatement(sqlWithCriteria);

            prepared_statement->setString(1, value);

            sql::ResultSet* result(prepared_statement->executeQuery());
            while (result->next()) {
                Customer* customer = customers.add_customers();
                fillCustomer(result, customer);
            }

            delete prepared_statement;
            delete result;

        } catch (sql::SQLException &e) {
            handleSQLException(e);
        }

        return customers;

    }


private:

    bool replace(string& str, const string& from, const string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    void fillCustomer(const sql::ResultSet* result, Customer* customer) const {
        customer->set_id(result->getInt("id"));
        customer->set_first_name(result->getString("first_name"));
        customer->set_last_name(result->getString("last_name"));
        customer->set_address(result->getString("address"));
        customer->set_gender(result->getString("gender"));
        customer->set_zip(result->getString("zip"));
        customer->set_city(result->getString("city"));
        customer->set_state(result->getString("state"));
        customer->set_country(result->getString("country"));
        customer->set_telephone(result->getString("telephone"));
        customer->set_email(result->getString("email"));
    }


    void reconnectWhenNeeded() const {
        if(!connection->isValid()){
            connection->reconnect();
        }
    }

    void handleSQLException(const sql::SQLException &e) const {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << method << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

};
