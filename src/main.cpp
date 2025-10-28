#include <iostream>
#include "../include/Database.h"
#include "../include/MySQLDatabase.h"
#include "../include/SQLiteDatabase.h"

using namespace std;

int main()
{

    // Create Objects
    string name[10]{"Alex", "Anna", "Peter", "Bob", "Olga", "Andrew", "Irina", "Georg", "Elena", "Samanta"};
    int age[10]{24, 25, 30, 40, 38, 24, 26, 27, 27, 42};
    vector<User> users;

    for (int i = 0; i < 10; i++)
    {

        User tmp{i + 1, name[i], age[i]};
        users.push_back(tmp);
    }

    // Class MySQLDatabase methods view
    {
        cout << "------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << "\t\t\tClass MySQLDatabase methods view:\n";
        cout << "------------------------------------------------------------------------------------------------------------------------------------------\n";
        Database *mysql_database = new MySQLDatabase("root", "simple1");
        cout << "\n\t1) Creating database Factory\n";
        mysql_database->createDatabase("Factory");

        // Creating other database / Using database / Dropping database
        cout << "\n\t2) Creating other database Example / Using database Example / Dropping database Exapmle\n";
        mysql_database->createDatabase("Example");
        mysql_database->useDatabase("Example");
        mysql_database->dropDatabase("Example");

        cout << "\n\t3) Using database Factory / Creating table Employee\n";
        // Using database /Creating table
        mysql_database->useDatabase("Factory");
        mysql_database->createTable("Employee");

        cout << "\n\t4) Table exist Employee?\n";
        // Exists table
        if (mysql_database->tableExists("Employee"))
            std::cout << "Table Employee exists \n";
        cout << "\n\t5) Rename table from Employee to Workers / Create table GoodWorkers and table BadWorkers\n";
        // Renaming table
        mysql_database->renameTable("Employee", "Workers");
        mysql_database->createTable("GoodWorkers");
        mysql_database->createTable("BadWorkers");
        cout << "\n\t6) Getting name of the tables from database\n";
        // Getting name of the tables from database
        auto tables_show = mysql_database->getTables();

        for (const auto &t : tables_show)
            cout << "Table: " << t << "\n";
        cout << "\n\t7) Dropping table BadWorkers. Getting name of the tables from database Factory?\n";
        // Dropping table
        mysql_database->dropTable("BadWorkers");
        // Getting name of the tables from database
        auto tables = mysql_database->getTables();

        for (const auto &t : tables)
            cout << "\tTable: " << t << "\n";

        cout << "\n\t8) Inserting data in table Workers\n";
        // Inserting data in table
        for (const auto &user : users)
            mysql_database->insert("Workers", user);
        cout << "\n\t9) Return all objects from table Workers\n";
        // Return all Objects from table
        auto allWorkers = mysql_database->getAllObjects("Workers");
        for (const auto &w : allWorkers)
            cout << "\t id: " << w.id << " " << w.name << " age: " << w.age << "\n";
        cout << "\n\t10) Return objects from table Workers , due to : name==Alex; age > 35\n";
        // Return Objects due to condition
        auto group_1 = mysql_database->getObjects("Workers", "name='Alex'");
        auto group_2 = mysql_database->getObjects("Workers", "age>35");

        for (const auto &g : group_1)
            cout << "\tid: " << g.id << " " << g.name << " age: " << g.age << "\n";

        cout << "\n";

        for (const auto &g : group_2)
            cout << "\tid: " << g.id << " " << g.name << " age: " << g.age << "\n";

        cout << "\n\t11) Remove object from table Workers due to condition: age > 40. Return all objects\n";
        // Remove Object from table due to condition
        mysql_database->removeObject("Workers", "age>40");

        // Return all Objects from table

        for (const auto &w : mysql_database->getAllObjects("Workers"))
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t12) Updating data in table Workers. From  id=1 (Alex age: 24) to Tim (id=1 age: 32)\n";
        // Updating data in table
        User temp{1, "Tim", 32};
        mysql_database->update("Workers", temp, "id=1");
        // Return all Objects from table

        for (const auto &w : mysql_database->getAllObjects("Workers"))
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t13) Order objects by one field Workers: field - age\n";
        // Order objects  by one field
        auto mysql_orderOneField = mysql_database->getObjectsOrderBy("Workers", "age");
        for (const auto &orderOneField : mysql_orderOneField)
            cout << "\tid: " << orderOneField.id << " " << orderOneField.name << " age: " << orderOneField.age << "\n";
        cout << "\n\t14) Order objects by two field Workers: first field - age, second field - name\n";
        // Order  objects by two field
        auto mysql_orderTwoField = mysql_database->getObjectsOrderBy("Workers", "age", "name");
        for (const auto &orderTwoField : mysql_orderTwoField)
            cout << "\tid: " << orderTwoField.id << " " << orderTwoField.name << " age: " << orderTwoField.age << "\n";
        cout << "\n\t15) Copying all objects from Workers to GoodWorkers\n";
        // Copying data from table1 to table2
        mysql_database->copyAllObjectsInOtherTable("Workers", "GoodWorkers");
        cout << "Getting all objects GoodWorkers:\n";
        auto goodWorkers = mysql_database->getAllObjects("GoodWorkers");

        for (const auto &w : goodWorkers)
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t16) Removing all objects from GoodWorkers / Get count objects table\n";
        // Removeing all  objects from table
        mysql_database->removeAllObjects("GoodWorkers");

        cout << "Count : " << mysql_database->getObjectsCount("GoodWorkers");
        cout << "\n\t17) Copying data from table Workers to GoodWorkers duo to condition: age > 26 && age < 40 + commit\n";
        // Copying data from table duo to condition + commit
        mysql_database->copyObjectsInOtherTable("Workers", "GoodWorkers", "age>26 && age<40");
        cout << "Getting all objects GoodWorkers:\n";
        auto bestWorkers = mysql_database->getAllObjects("GoodWorkers");

        for (const auto &w : bestWorkers)
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";
        cout << "\n\t18) Start Transaction / Removing All Objects from Workers / Rollback\n";
        // Start transaction + RemoveAllObjects + Rollback
        mysql_database->beginTransaction();
        mysql_database->removeAllObjects("Workers");

        mysql_database->rollback();
        cout << "\n\t19) Getting objects count from table Workers\n";
        // Getting objects count from the table
        cout << "Objects count : " << mysql_database->getObjectsCount("Workers") << "\n";
        cout << "\n\t20) Get all objects from table Workers / Swapping objects from table Workers: id=1 , id=8\n";
        // Return all Objects from table

        for (const auto &w : mysql_database->getAllObjects("Workers"))
            cout << "\tid:" << w.id << " " << w.name << " age: " << w.age << "\n";
        // Swapping objects
        mysql_database->swapObjects("Workers", "id=1", "id=8");
        // Return all Objects from table

        for (const auto &w : mysql_database->getAllObjects("Workers"))
            cout << "\tid:" << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t21) Database Factory deleted\n";
        mysql_database->dropDatabase("Factory");
    }

    // Class SQLiteDatabase methods view
    {
        cout << "------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << "\t\t\tClass SQLiteDatabase methods view:\n";
        cout << "------------------------------------------------------------------------------------------------------------------------------------------\n";
        Database *sqlite_database = new SQLiteDatabase();
        cout << "\n\t1) Creating database Factory\n";
        sqlite_database->createDatabase("Factory");

        // Creating other databse / Using database / Dropping database
        cout << "\n\t2) Creating other database Example / Using database Example / Dropping database Exapmle\n";
        sqlite_database->createDatabase("Example");
        sqlite_database->useDatabase("Example");
        sqlite_database->dropDatabase("Example");

        cout << "\n\t3) Using database Factory / Creating table Employee\n";
        // Using database /Creating table
        sqlite_database->useDatabase("Factory");
        sqlite_database->createTable("Employee");

        cout << "\n\t4) Table exist Employee?\n";
        // Exists table
        if (sqlite_database->tableExists("Employee"))
            std::cout << "Table Employee exists \n";

        cout << "\n\t5) Rename table from Employee to Workers / Create table GoodWorkers and table BadWorkers\n";
        // Renaming table
        sqlite_database->renameTable("Employee", "Workers");
        sqlite_database->createTable("GoodWorkers");
        sqlite_database->createTable("BadWorkers");

        cout << "\n\t6) Getting name of the tables from database\n";
        // Getting name of the tables from database
        auto tables = sqlite_database->getTables();

        for (const auto &t : tables)
            cout << "Table: " << t << "\n";

        cout << "\n\t7) Dropping table BadWorkers. Getting name of the tables from database Factory?\n";
        // Dropping table
        sqlite_database->dropTable("BadWorkers");
        for (const auto &t : sqlite_database->getTables())
            cout << "Table: " << t << "\n";

        cout << "\n\t8) Inserting data in table Workers\n";
        // Inserting data in table
        for (const auto &user : users)
            sqlite_database->insert("Workers", user);

        cout << "\n\t9) Return all objects from table Workers\n";
        // Return all Objects from table

        for (const auto &w : sqlite_database->getAllObjects("Workers"))
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t10) Return objects from table Workers , due to : name==Alex; age > 35\n";
        // Return Objects due to condition
        auto group_1 = sqlite_database->getObjects("Workers", "name='Alex'");
        auto group_2 = sqlite_database->getObjects("Workers", "age>35");

        for (const auto &g : group_1)
            cout << "\tid: " << g.id << " " << g.name << " age: " << g.age << "\n";

        cout << "\n";

        for (const auto &g : group_2)
            cout << "\tid: " << g.id << " " << g.name << " age: " << g.age << "\n";

        cout << "\n\t11) Remove object from table Workers due to condition: age > 40. Return all objects\n";
        // Remove Object from table due to condition
        sqlite_database->removeObject("Workers", "age>40");
        // Return all Objects from table
        for (const auto &w : sqlite_database->getAllObjects("Workers"))
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t12) Updating data in table Workers. From  id=1 (Alex age: 24) to Tim (id=1 age: 32)\n";
        // Updating data in table
        User temp{1, "Tim", 32};
        sqlite_database->update("Workers", temp, "id=1");
        // Return all Objects from table
        for (const auto &w : sqlite_database->getAllObjects("Workers"))
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t13) Order objects by one field Workers: field - age\n";
        // Order objects  by one field
        auto sqlite_orderOneField = sqlite_database->getObjectsOrderBy("Workers", "age");
        for (const auto &orderOneField : sqlite_orderOneField)
            cout << "\tid: " << orderOneField.id << " " << orderOneField.name << " age: " << orderOneField.age << "\n";

        cout << "\n\t14) Order objects by two field Workers: first field - age, second field - name\n";
        // Order  objects by two field
        auto mysql_orderTwoField = sqlite_database->getObjectsOrderBy("Workers", "age", "name");
        for (const auto &orderTwoField : mysql_orderTwoField)
            cout << "\tid: " << orderTwoField.id << " " << orderTwoField.name << " age: " << orderTwoField.age << "\n";

        cout << "\n\t15) Copying all objects from Workers to GoodWorkers\n";
        // Copying data from table1 to table2
        sqlite_database->beginTransaction();
        sqlite_database->copyAllObjectsInOtherTable("Workers", "GoodWorkers");

        auto goodWorkers = sqlite_database->getAllObjects("GoodWorkers");

        for (const auto &w : goodWorkers)
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t16) Removing all objects from GoodWorkers / Get count objects table\n";
        // Removeing all  objects from table
        sqlite_database->removeAllObjects("GoodWorkers");
        cout << "Count : " << sqlite_database->getObjectsCount("GoodWorkers");

        cout << "\n\t17) Copying data from table Workers to GoodWorkers duo to condition: age > 26 && age < 40 + commit\n";
        // Copying data from table duo to condition + commit
        sqlite_database->beginTransaction();
        sqlite_database->copyObjectsInOtherTable("Workers", "GoodWorkers", "age>26 AND age<40");
        cout << "Getting all objects GoodWorkers:\n";
        auto bestWorkers = sqlite_database->getAllObjects("GoodWorkers");

        for (const auto &w : bestWorkers)
            cout << "\tid: " << w.id << " " << w.name << " age: " << w.age << "\n";

        cout << "\n\t18) Start Transaction / Removing All Objects from Workers / Rollback\n";
        // Start transaction + RemoveAllObjects + Rollback
        sqlite_database->beginTransaction();
        sqlite_database->removeAllObjects("Workers");
        sqlite_database->rollback();

        cout << "\n\t19) Getting objects count from table Workers\n";
        // Getting objects count from the table
        cout << "Objects count : " << sqlite_database->getObjectsCount("Workers") << "\n";
        cout << "\n\t20) Get all objects from table Workers / Swapping objects from table Workers: id=1 , id=8\n";
        // Return all Objects from table

        for (const auto &w : sqlite_database->getAllObjects("Workers"))
            cout << "\tid:" << w.id << " " << w.name << " age: " << w.age << "\n";
        // Swapping objects
        sqlite_database->swapObjects("Workers", "id=1", "id=8");
        // Return all Objects from table

        for (const auto &w : sqlite_database->getAllObjects("Workers"))
            cout << "\tid:" << w.id << " " << w.name << " age: " << w.age << "\n";
        cout << "\n\t21) Database Factory deleted\n";
        sqlite_database->dropDatabase("Factory");
    }

    return 0;
}
