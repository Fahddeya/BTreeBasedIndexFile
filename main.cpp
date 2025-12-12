
#include <bits/stdc++.h>
using namespace std;
fstream index;
int m = 5;
int rows = 10;
int sizeOfEachField = 5;
int recordLen = (2 * m + 1) * sizeof(int) ;
void initFile();


void CreateIndexFileFile (char* filename, int numberOfRecords, int keys)
{
    index.open(filename, ios::in | ios::out | ios::binary);
    if (!index)
    {
        index.open(filename, ios::app | ios::binary);
        index.close();
        index.open(filename, ios::in | ios::out | ios::binary);
    }
    rows = numberOfRecords;
    m = keys;
    recordLen = (2 * m + 1) * sizeof(int);
    initFile();
}

void OpenFile(char* filename){
    index.open(filename, ios::in | ios::out | ios::binary);
    if (!index)
    {
        index.open(filename, ios::app | ios::binary);
        index.close();
        index.open(filename, ios::in | ios::out | ios::binary);
    }
}

int getDataFromFile(int row, int column)
{
    index.seekg(recordLen * row + column * sizeof(int), ios::beg);
    int wantedData;
    index.read(reinterpret_cast<char *>(&wantedData), sizeof(int));
    return wantedData;
}
string handleNumber(int element)
{
    return "";
}
vector<pair<int, int>> allData(int Row)
{
    int Column = 1;
    vector<pair<int, int>> nodeData;
    while (Column != (2 * m + 1))
    {
        if (getDataFromFile(Row, Column) == -1 || getDataFromFile(Row, Column) == -1)
        {
            break;
        }
        nodeData.push_back({getDataFromFile(Row, Column), getDataFromFile(Row, Column + 1)});
        Column += 2;
    }
    return nodeData;
}
void setDataInFile(int element, int row, int column)
{
    index.seekp(recordLen * row + column * sizeof(int), ios::beg);
    index.write(reinterpret_cast<const char *>(&element), sizeof(int));
    index.flush();
}
void initFile()
{
    // make all rows and columns with -1
    int cnt = 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < m * 2 + 1; j++)
        {
            if (j == 1)
            {
                if (cnt == rows)
                {
                    cnt = -1;
                }
                index.write(reinterpret_cast<const char *>(&cnt), sizeof(int));
                cnt++;
                continue;
            }
            int val = -1;
            index.write(reinterpret_cast<const char *>(&val), sizeof(int));
        }
    }
    index.flush();
}
void checkFile()
{
    // make the file able to use
    // if the file can't be use it will be set all with -1
    index.seekg(0, ios::end);
    int end = index.tellg();
    index.seekg(0);
    int sizeOfFile = rows * (2 * m + 1) * sizeof(int);
    if (end != sizeOfFile)
    {
        // fill all with -1
        index.seekp(0);
        initFile();
    }
}
void DisplayIndexFileContent()
{
    index.seekg(0);
    int countOfElements = 0;
    while (index.tellg() < (2 * m + 1) * rows * sizeof(int))
    {
        int temp;
        index.read(reinterpret_cast<char *>(&temp), sizeof(int));
        countOfElements++;
        cout << temp << " ";
        if (countOfElements % (2 * m + 1) == 0)
        {
            cout << "\n";
        }
        if (countOfElements == (2 * m + 1) * rows)
            break;
    }
    cout << "\n";
}
void write(vector<pair<int, int>> record, int Row, int state)
{
    setDataInFile(state, Row, 0);
    int Column = 1;
    for (auto i : record)
    {
        setDataInFile(i.first, Row, Column);
        setDataInFile(i.second, Row, Column + 1);
        Column += 2;
    }
    while (Column < (2 * m + 1))
    {
        setDataInFile(-1, Row, Column);
        setDataInFile(-1, Row, Column + 1);
        Column += 2;
    }
}
int search(int indexNumber, int Row)
{
    int Column = 1;

    vector<pair<int, int>> nodeData = allData(Row);
    int ret = -1;

    if (getDataFromFile(Row, 0) == 0)
    {
        for (auto i : nodeData)
        {
            if (i.first == indexNumber)
            {
                ret = i.second;
            }
        }
    }
    else
    {
        for (auto i : nodeData)
        {
            if (i.first >= indexNumber)
            {
                ret = search(indexNumber, i.second);
                break;
            }
        }
    }
    return ret;
}
int SearchARecord(int indexNumber)
{
    return search(indexNumber, 1);
}
int toAdd;
int insetAfterSplit(int indexNumber, int Row)
{
    int Column = 1;
    vector<pair<int, int>> nodeData;
    while (Column != (2 * m + 1))
    {
        if (getDataFromFile(Row, Column) == -1)
        {
            break;
        }
        nodeData.push_back({getDataFromFile(Row, Column), getDataFromFile(Row, Column + 1)});
        Column += 2;
    }
    int firstEmpty = getDataFromFile(0, 1);
    int toAddOffset;
    for (auto &i : nodeData)
    {
        if (i.first > indexNumber)
        {
            toAddOffset = i.second;
            i.second = firstEmpty - 1;
            break;
        }
    }
    nodeData.push_back({indexNumber, toAddOffset});
    sort(nodeData.begin(), nodeData.end());
    if (nodeData.size() > m)
    {
        vector<pair<int, int>> firstSplit, secondSplit;
        for (int i = 0; i < nodeData.size(); i++)
        {
            if (i >= nodeData.size() / 2)
            {
                secondSplit.push_back(nodeData[i]);
            }
            else
            {
                firstSplit.push_back(nodeData[i]);
            }
        }
        if (Row == 1)
        {
            // root
            int firstEmpty = getDataFromFile(0, 1);
            if (firstEmpty == -1)
            {
                cout << "Can't Add" << "\n";
                return -1;
            }
            int secondEmpty = getDataFromFile(0, 1) + 1;
            if (secondEmpty + 1 == 10)
            {
                setDataInFile(-1, 0, 1);
            }
            else
            {
                setDataInFile(secondEmpty + 1, 0, 1);
            }
            vector<pair<int, int>> root;
            root.push_back({firstSplit[firstSplit.size() - 1].first, firstEmpty});
            root.push_back({secondSplit[secondSplit.size() - 1].first, secondEmpty});
            write(root, 1, 1);
            write(firstSplit, firstEmpty, getDataFromFile(Row, 0));
            write(secondSplit, secondEmpty, getDataFromFile(Row, 0));
            toAdd = -1;
        }
        else
        {
            int firstEmpty = getDataFromFile(0, 1);
            if (firstEmpty == -1)
            {
                cout << "Can't Add" << "\n";
                return -1;
            }
            setDataInFile(firstEmpty + 1, 0, 1);
            write(secondSplit, firstEmpty, 0);
            write(firstSplit, Row, getDataFromFile(Row, 0));
            toAdd = firstSplit.back().first;
        }
    }
    else
    {
        write(nodeData, Row, getDataFromFile(Row, 0));
        toAdd = -1;
        return 1;
    }
    return 0;
}
int insertInIndex(int indexNumber, int offset, int Row)
{
    int Column = 1;
    // get all the data of the node and sort it to inset the new element node
    vector<pair<int, int>> nodeData = allData(Row);
    int ret = -1;
    if (getDataFromFile(Row, 0) == 0)
    {
        // leaf node
        nodeData.push_back({indexNumber, offset});
        // we can use set to sort i will make it in the upcoming update
        sort(nodeData.begin(), nodeData.end());
        if (nodeData.size() > m)
        {
            // will split ;
            vector<pair<int, int>> firstSplit, secondSplit;
            for (int i = 0; i < nodeData.size(); i++)
            {
                if (i >= nodeData.size() / 2)
                {
                    secondSplit.push_back(nodeData[i]);
                }
                else
                {
                    firstSplit.push_back(nodeData[i]);
                }
            }
            if (Row == 1)
            {
                int firstEmpty = getDataFromFile(0, 1);
                if (firstEmpty == -1)
                {
                    return ret;
                }
                int secondEmpty = getDataFromFile(0, 1) + 1;
                setDataInFile(secondEmpty + 1, 0, 1);
                vector<pair<int, int>> root;
                root.push_back({firstSplit[firstSplit.size() - 1].first, firstEmpty});
                root.push_back({secondSplit[secondSplit.size() - 1].first, secondEmpty});
                write(root, 1, 1);
                write(firstSplit, firstEmpty, 0);
                write(secondSplit, secondEmpty, 0);
                if (find(firstSplit.begin(), firstSplit.end(), make_pair(indexNumber, offset)) != firstSplit.end())
                {
                    ret = firstEmpty;
                }
                else
                {
                    ret = secondEmpty;
                }
            }
            else
            {
                // split will differ
                int firstEmpty = getDataFromFile(0, 1);
                if (firstEmpty == -1)
                {
                    return ret;
                }
                setDataInFile(firstEmpty + 1, 0, 1);
                write(secondSplit, firstEmpty, 0);
                write(firstSplit, Row, 0);
                toAdd = firstSplit.back().first;
                if (find(firstSplit.begin(), firstSplit.end(), make_pair(indexNumber, offset)) != firstSplit.end())
                {
                    ret = Row;
                }
                else
                {
                    ret = firstEmpty;
                }
            }
        }
        else
        {
            write(nodeData, Row, 0);
            ret = Row;
        }
    }
    else
    {
        // internal node and will go to another node
        bool foundLargerThanMe = false;
        for (auto i : nodeData)
        {
            if (i.first > indexNumber)
            {
                foundLargerThanMe = true;
                ret = insertInIndex(indexNumber, offset, i.second);
                if (toAdd != -1)
                {
                    insetAfterSplit(toAdd, Row);
                }
                break;
            }
        }
        if (!foundLargerThanMe)
        {
            nodeData[nodeData.size() - 1].first = indexNumber;
            ret = insertInIndex(indexNumber, offset, nodeData.back().second);
            if (ret != -1)
            {
                write(nodeData, Row, 1);
                if (toAdd != -1)
                {
                    insetAfterSplit(toAdd, Row);
                }
            }
        }
    }
    return ret;
}
int InsertNewRecordAtIndex(int RecordID, int Reference)
{
    if (SearchARecord(RecordID) == -1)
    {
        
        if (getDataFromFile(1, 0) == -1)
        {
            // means that the index file is empty
            setDataInFile(0, 1, 0);
            setDataInFile(RecordID, 1, 1);
            setDataInFile(Reference, 1, 2);
            setDataInFile(2, 0, 1);
            return 1;
        }
        else
        {
            toAdd = -1;
            return insertInIndex(RecordID, Reference, 1);
        }
    }
    else
    {
        return -1;
    }
}
bool writeRecord = true;
int deleteIndex(int indexNumber, int Row, int parentRow)
{
    
    int ret = -1;
    vector<pair<int, int>> nodeData = allData(Row);
    if (getDataFromFile(Row, 0) == 0)
    {
        // leaf
        if (nodeData.size() > m / 2)
        {
            // hear we can delete a node
            int count = 0;
            for (auto i : nodeData)
            {
                if (i.first == indexNumber)
                {
                    nodeData.erase(nodeData.begin() + count);
                    break;
                }
                count++;
            }
            write(nodeData, Row, getDataFromFile(Row, 0));
            ret = nodeData.back().first;
            return ret;
        }
        else
        {
            if (parentRow != -1)
            {
                vector<pair<int, int>> dataOfParent = allData(parentRow);
                vector<pair<int, int>> siblings;
                for (auto i : dataOfParent)
                {
                    siblings.push_back({i.first, i.second});
                }
                pair<int, int> leftSibling, rightSibling;
                int left = -1, right = -1;
                for (int i = 0; i < siblings.size(); i++)
                {
                    if (siblings[i].first == nodeData.back().first)
                    {
                        if (i - 1 >= 0)
                        {
                            leftSibling = siblings[i - 1];
                            left = i - 1;
                        }
                        if (i + 1 < siblings.size())
                        {
                            rightSibling = siblings[i + 1];
                            right = i + 1;
                        }
                        break;
                    }
                }
            
                vector<pair<int, int>> leftSiblingChildren = allData(leftSibling.second);
                vector<pair<int, int>> rightSiblingChildren = allData(rightSibling.second);

                int count = 0;
                for (auto i : nodeData)
                {
                    if (i.first == indexNumber)
                    {
                        nodeData.erase(nodeData.begin() + count);
                        break;
                    }
                    count++;
                }
                if (leftSiblingChildren.size() > m / 2)
                {
                    nodeData.insert(nodeData.begin(), leftSiblingChildren.back());
                    leftSiblingChildren.erase(leftSiblingChildren.begin() + leftSiblingChildren.size() - 1);
                    write(nodeData, Row, getDataFromFile(Row, 0));
                    write(leftSiblingChildren, leftSibling.second, getDataFromFile(leftSibling.second, 0));
                    siblings[left].first = leftSiblingChildren.back().first;
                    siblings[left + 1].first = nodeData.back().first;
                    write(siblings, parentRow, getDataFromFile(parentRow, 0));
                    ret = nodeData.back().first;
                    writeRecord = false;
                    return ret;
                }
                else if (rightSiblingChildren.size() > m / 2)
                {
                    // make the same but on the right sibling
                    nodeData.insert(nodeData.begin(), rightSiblingChildren.front());
                    rightSiblingChildren.erase(rightSiblingChildren.begin());
                    write(nodeData, Row, getDataFromFile(Row, 0));
                    write(rightSiblingChildren, rightSibling.second, getDataFromFile(rightSibling.second, 0));
                    siblings[right].first = rightSiblingChildren.back().first;
                    siblings[right - 1].first = nodeData.back().first;
                    write(siblings, parentRow, getDataFromFile(parentRow, 0));
                    ret = nodeData.back().first;
                    writeRecord = false;
                    return ret;
                }
                else
                {
                    
                    for (auto i : nodeData)
                    {
                        leftSiblingChildren.push_back(i);
                    }
                    if (left != -1)
                    {
                        siblings[left].first = leftSiblingChildren.back().first;
                        nodeData.clear();
                        write(nodeData, Row, -1);
                        setDataInFile(Row, 0, 1);
                        siblings.erase(siblings.begin() + left + 1);
                        write(leftSiblingChildren, leftSibling.second, getDataFromFile(leftSibling.second, 0));
                        write(siblings, parentRow, getDataFromFile(parentRow, 0));
                        ret = leftSiblingChildren.back().first;
                        writeRecord = false;
                        return ret;
                    }
                    else
                    {
                        // merge the right
                        // doctor didn't mention it
                    }
                }
            }
            else
            {
                // hear we can delete a node
                int count = 0;
                for (auto i : nodeData)
                {
                    if (i.first == indexNumber)
                    {
                        nodeData.erase(nodeData.begin() + count);
                        break;
                    }
                    count++;
                }
                if (nodeData.empty())
                {
                    write(nodeData, Row, -1);
                    setDataInFile(Row, 0, 1);
                    return -1;
                }
                else
                {
                    write(nodeData, Row, getDataFromFile(Row, 0));
                    ret = nodeData.back().first;
                    return ret;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < nodeData.size(); i++)
        {
            if (nodeData[i].first >= indexNumber)
            {
                ret = deleteIndex(indexNumber, nodeData[i].second, Row);
                if (ret == -1)
                {
                    nodeData.erase(nodeData.begin() + i);
                }
                else
                {
                    nodeData[i].first = ret;
                }
                break;
            }
        }
    }

    if (nodeData.empty())
    {
        write(nodeData, Row, -1);
        setDataInFile(Row, 0, 1);
        return -1;
    }
    else
    {
        ret = nodeData.back().first;
        if (writeRecord)
        {
            write(nodeData, Row, getDataFromFile(Row, 0));
        }
        else
        {
            writeRecord = true;
        }
        return ret;
    }
}
void DeleteRecordFromIndex(int indexNumber)
{
    deleteIndex(indexNumber, 1, -1);
}
void completedTests()
{
    checkFile();
    cout << InsertNewRecordAtIndex(3, 12) << "\n";
    cout << InsertNewRecordAtIndex(7, 24) << "\n";
    cout << InsertNewRecordAtIndex(10, 48) << "\n";
    cout << InsertNewRecordAtIndex(24, 60) << "\n";
    cout << InsertNewRecordAtIndex(14, 72) << "\n";
    DisplayIndexFileContent();
    cout << "\n\n";
    cout << InsertNewRecordAtIndex(19, 84) << "\n";
    DisplayIndexFileContent();
    cout << "\n\n";
    cout << InsertNewRecordAtIndex(30, 196) << "\n";
    cout << InsertNewRecordAtIndex(15, 108) << "\n";
    cout << InsertNewRecordAtIndex(1, 120) << "\n";
    cout << InsertNewRecordAtIndex(5, 132) << "\n";
    DisplayIndexFileContent();
    cout << "\n\n";
    cout << InsertNewRecordAtIndex(2, 144) << "\n";
    DisplayIndexFileContent();
    cout << "\n\n";
    cout << InsertNewRecordAtIndex(8, 156) << "\n";
    cout << InsertNewRecordAtIndex(9, 168) << "\n";
    cout << InsertNewRecordAtIndex(6, 180) << "\n";
    cout << InsertNewRecordAtIndex(11, 192) << "\n";
    cout << InsertNewRecordAtIndex(12, 204) << "\n";
    cout << InsertNewRecordAtIndex(17, 216) << "\n";
    cout << InsertNewRecordAtIndex(18, 228) << "\n";
    DisplayIndexFileContent();
    cout << "\n\n";
    cout << InsertNewRecordAtIndex(32, 240) << "\n";
    DisplayIndexFileContent();
    cout << "\n\n";
    // more test to out -1
    // cout << InsertNewRecordAtIndex(13 , 40) << "\n" ;
    // cout << InsertNewRecordAtIndex(16 , 40) << "\n" ;
    // cout << InsertNewRecordAtIndex(20 , 40) << "\n" ;
    // cout << InsertNewRecordAtIndex(21 , 40) << "\n" ;
    // cout << InsertNewRecordAtIndex(22 , 40) << "\n" ;
    cout << "deleting 10\n";
    DeleteRecordFromIndex(10);
    DisplayIndexFileContent();
    cout << "\n\n";

    cout << "deleting 9\n";
    DeleteRecordFromIndex(9);
    DisplayIndexFileContent();
    cout << "\n\n";

    cout << "deleting 8\n";
    DeleteRecordFromIndex(8);
    DisplayIndexFileContent();
    cout << "\n\n";
    
    cout <<"ref for key 3 :" << SearchARecord(3) << "\n";
    cout <<"ref for key 11 :" << SearchARecord(11) << "\n";
    cout <<"ref for key 50 :" << SearchARecord(50) << "\n";
    
}
int main()
{
    const char* filename = "indexfile.bin";
    cout << "1) new file\n";
    cout << "2) open file with default number of rows and keys\n";
    int choice ;
    cin >> choice;
    if(choice == 1){
        CreateIndexFileFile ((char*)filename, 10, 5 );
    }

    else {
        OpenFile((char*)filename);
    }
    do
    {
        cout << "1) add \n";
        cout << "2) remove\n";
        cout << "3) search\n";
        cout << "4) test case\n";
        cout << "5) exit\n";
        cin >> choice;
        int key , ref;
        switch (choice)
        {
        case 1:
            cout << "key : ";
            cin >> key;
            cout << "ref : ";
            cin  >> ref ;
            cout << InsertNewRecordAtIndex(key, ref) << endl;
            break;
        case 2:
            cout << "key : ";
            cin >> key;
            DeleteRecordFromIndex(key);
            break;
        case 3:
            cout << "key : ";
            cin >> key;
            cout << SearchARecord(key) << endl;
            break;
        case 4:
            completedTests();
            break;
        default:
            cout << "error\n";
            break;
        }
    } while (choice != 5);
    index.close();
}
