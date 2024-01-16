#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <list>
#include <fstream>
#include <iomanip>
#include<iterator>
#include<stack>
#include<deque>
#include<vector>
using namespace std;
vector<char> str;
std::vector<char>  deleteWords;

void gotoxy(int x, int y)
{
	COORD coordinates;
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

class node
{
public:
	char c;
	node* next;
	node* prev;
};

class editor
{
public:
	int x, y, p;
	string menu[4];
	string copy, message;
	list<node*> line;
	list<string> fileList;
	
	node *s, *f, *t;
	node* f31, * f32;
	char input;

	// Iterator for the current line
	list<node*>::iterator lineIterator;
	//stack
	

	void run();
	void clear();
	void display();
	void backspace();
	void arrowKey();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void updateX();
	void enter();
	void fKey();
	void text();
	void showMenu();
	void format();
	void deleteNode();
	void displayMenu();
	void storeFileName(const string& fileName);
	void displayFileList();
	void viewStoredFileNames();
	void saveFileList(const list<string>& fileName);
	list<string> loadFileList();
	void saveText();
	void loadText();
	void undo();
	void redo();
	//void deleteAllNodes(list<node*>& nodeList);
	//void copyState(const list<node*>& source, list<node*>& destination);
	


	editor()
	{
		x = 0, y = 0;
		s = new node, f = new node, t = new node;
		menu[0] = menu[1] = menu[2] = "OFF";
		menu[3] = "OK";
		f->prev = t->next = f31 = f32 = nullptr;
		f->next = t->prev = s;
		s->prev = f;
		s->next = t;
		s->c = ' ';
		copy = message = "";
		line.push_back(f);
		lineIterator = line.begin();
		display();
	}

	~editor() { deleteNode(); }
};
void editor::saveText()
{
	system("CLS");
	cout << "enter file name \n(no need to include .txt, we will add it for you)\n: ";
	string fileName = "";
	cin >> fileName;
	fileName += ".txt";
	ofstream save(fileName); // make new file
	if (save.is_open())
	{
		for (node* ptr : line)
		{
			while (ptr->next->next != nullptr)
			{
				if (ptr->next != s)
					save << ptr->next->c;
				ptr = ptr->next;
			}
			save << endl;
		}
		save.close();
		message = "file " + fileName + " saved successfully";
		storeFileName(fileName);
		saveFileList(fileList);
	}
	else
	{
		message = "fail to save file " + fileName;
	}
}
void editor::loadText()
{
	system("CLS");
	cout << "enter file name you want to load \n(no need to include .txt, we will add it for you)\n: ";
	string fileName;
	cin >> fileName;
	fileName += ".txt";
	ifstream load(fileName);
	if (load.is_open())
	{
		deleteNode();
		string str;
		x = 0, y = 0;
		s = new node;
		s->prev = nullptr;
		s->c = ' ';
		while (getline(load, str)) // load text into program
		{
			t = new node;
			f = new node;
			if (s->prev == nullptr) // for first line
			{
				f->next = s;
				f->prev = nullptr;
				s->prev = f;
				s->next = t;
				t->prev = s;
				t->next = nullptr;
				line.push_back(f);
				storeFileName(fileName);
			}
			else // for other line
			{
				s->prev->next = t;
				t->prev = s->prev;
				t->next = nullptr;
				f->prev = nullptr;
				f->next = s;
				s->prev = f;
				line.push_back(f);
				y++;
				x = 0;
				storeFileName(fileName);
			}
			for (char c : str) // put charater into each line
			{
				t = new node;
				t->c = c;
				s->prev->next = t;
				t->prev = s->prev;
				s->prev = t;
				t->next = s;
				x++;
			}
		}
		load.close();
		message = "file " + fileName + " loaded successfully";
	}
	else
		message = "file not found";

}
void editor::deleteNode()
{
	// Check if the iterator is at the end of the list
	if (lineIterator != line.end())
	{
		node* ptr = *lineIterator;

		// Save the next pointer before deleting the current node
		t = ptr->next;

		// Delete the entire line pointed by the iterator
		while (ptr != nullptr)
		{
			node* nextNode = ptr->next;
			delete ptr;
			ptr = nextNode;
		}

		// Erase the line from the list
		lineIterator = line.erase(lineIterator);
	}
	else
	{
		return;
	}
}

void editor::storeFileName(const string& fileName)
{
	fileList.push_back(fileName);
}

void editor::displayFileList()
{
	system("CLS");
	cout << "List of Stored Files:\n";

	for (const string& fileName : fileList)
	{
		cout << "- " << fileName << endl;
	}

	_getch(); // Wait for user input before clearing the screen
}



void editor::arrowKey()
{
	int key = _getch();
	switch (key)
	{
	case 75: // left arrow key
		moveLeft();
		break;
	case 77: // right arrow key
		moveRight();
		break;
	case 72: // up arrow key
		moveUp();
		break;
	case 80: // down arrow key
		moveDown();
		break;
	default:
		message = "Arrow key function unavailable: unknown key.";
		break;
	}
}

void editor::moveLeft()
{
	if (s->prev != f)
	{
		s->next->prev = s->prev;
		s->prev->next = s->next;
		s->prev = s->prev->prev;
		s->prev->next = s;
		s->next = s->next->prev;
		s->next->prev = s;
		x--;
	}
	else
	{
		message = "Left arrow key function unavailable: out of bound.";
	}
}

void editor::moveRight()
{
	if (s->next->next != nullptr)
	{
		s->prev->next = s->next;
		s->prev->next->prev = s->prev;
		s->next = s->next->next;
		s->next->prev = s;
		s->prev = s->prev->next;
		s->prev->next = s;
		x++;
	}
	else
	{
		message = "Right arrow key function unavailable: out of bound.";
	}
}

void editor::moveUp()
{
	if (f->prev != nullptr)
	{
		f = f->prev;
		updateX();
		y--;
	}
	else
	{
		message = "Up arrow key function unavailable: out of bound.";
	}
}

void editor::moveDown()
{
	if (f->next != nullptr)
	{
		f = f->next;
		updateX();
		y++;
	}
	else
	{
		message = "Down arrow key function unavailable: out of bound.";
	}
}

void editor::updateX()
{
	int newX = 0;
	auto t = f;
	while (x-- != 0 && t->next != nullptr)
	{
		newX++;
		t = t->next;
	}

	x = newX;
}



void editor::backspace()
{
	if (s->prev != f) // for normal deletion
	{
		auto toDelete = s->prev;
		s->prev = s->prev->prev;
		s->prev->next = s;
		delete toDelete;
		x--;
	}
	else if (s->prev != (*line.begin())) // if it is the first node in the line
	{
		x = 0;
		auto itPrev = std::prev(lineIterator, 1);

		if (itPrev != line.begin())
		{
			f = *itPrev;
			t = f;
			while (t->next->next != nullptr)
			{
				t = t->next;
				x++;
			}

			auto itErase = line.erase(itPrev);
			lineIterator = itErase;
			deleteNode(); // Use deleteNode to delete the entire line

			if (s->prev)
			{
				s->prev = t;
				t->next = s;
				y--;
			}
		}
		else // out of bound
		{
			message = "Backspace key function unavailable: out of bound.";
		}
	}
	else // out of bound
	{
		message = "Backspace key function unavailable: out of bound.";
	}
}
list<string> editor::loadFileList()
{
	list<string> fileList;  // Create an empty list to store file names.
	ifstream file("fileList.txt");  // Open the file named "fileList.txt" for reading.

	if (file.is_open()) {
		string fileName;
		while (getline(file, fileName)) {
			fileList.push_back(fileName);  // Add each file name to the list.
		}
		file.close();  // Close the file after reading.
	}
	else {
		cerr << "Unable to open the file for loading the file list." << endl;
	}

	return fileList;  // Return the list of file names.
}
void editor::saveFileList(const list<string>& fileList)
{
	ofstream file("fileList.txt");  // Open the file named "fileList.txt" for writing.

	if (file.is_open()) {
		for (const string& fileName : fileList) {
			file << fileName << endl;  // Write each file name to the file, followed by a newline.
		}
		file.close();  // Close the file after writing.
		cout << "File list saved successfully." << endl;
	}
	else {
		cerr << "Unable to open the file for saving the file list." << endl;
	}
}
void editor::enter()
{
	t = new node; // new node for the end of the old line
	s->prev->next = t;
	t->prev = s->prev;
	t->next = nullptr;

	// Find the line at position (y + 1)
	auto it = line.begin();
	advance(it, y + 1);

	f = new node; // new node for the start of the new line
	f->prev = nullptr;
	f->next = s;
	s->prev = f;

	// Insert the new element at the specified position
	line.insert(it, f);

	x = 0;
	y++;
}

void editor::fKey()
{
	switch (_getch())
	{
	case (59): // f1 save text into file
	{
		saveText();
	}
	break;
	case 60: // f2 load file into program
	{
		loadText();
	}

	break;
	case (61): // f3 save
	{
		int pos = 0;
		if (f31 == nullptr && f32 == nullptr) // when user presses f3 the first time, it sets the first location
		{
			f31 = s->next;
			pos = y;
			message = "first location acquired, now press f3 again at a later location to record a string";
		}
		else if (f31 != nullptr && f32 == nullptr) // for when user presses f3 the second time, it saves the string between the two locations as copy
		{
			f32 = s->prev;
			if (f32->prev == nullptr)
				f32 = f32->next;

			auto itStart = lineIterator;
			auto itEnd = lineIterator;

			// Move the iterator to the starting position
			advance(itStart, pos);

			// Move the iterator to the ending position
			advance(itEnd, y + 1);

			// Reset the copy string
			copy = "";

			// Iterate over the lines between the start and end positions
			for (auto it = itStart; it != itEnd; ++it)
			{
				node* currentNode = *it;

				// Iterate over the characters in the line
				while (currentNode->next->next != nullptr)
				{
					copy += currentNode->next->c;
					currentNode = currentNode->next;
				}

				// Add a newline character if it's not the last line
				if (it != prev(itEnd))
				{
					copy += "\n";
				}
			}

			message = "string recorded, your string is " + copy + " \npress f4 to paste the string copied\nthis string will stay in memory until you press f3 again to reset the string";
		}
		else if (f31 != nullptr && f32 != nullptr) // reset location when user presses f3 the third time
		{
			f31 = nullptr;
			f32 = nullptr;
			copy = "";
			message = "string has being reset. now press f3 to set first copy location";
		}
		break;
	}

	break;
	case (62): // f4 paste
	{
		for (char c : copy) // loop through each character in copy string, put each char as a node and insert into text
		{
			if (c == '\n') // start a new line if the character is \n
			{
				f = new node;
				t = new node;
				s->prev->next = t;
				t->prev = s->prev;
				t->next = nullptr;
				f->prev = nullptr;
				f->next = s;
				s->prev = f;
				line.insert(lineIterator, f);
				y++;
				x = 0;
			}
			else // for normal characters
			{
				t = new node;
				t->c = c;
				s->prev->next = t;
				t->prev = s->prev;
				s->prev = t;
				t->next = s;
				x++;
			}
		}
	}
	break;
	case (64):
		showMenu(); // go to the menu page for f6
	}
}

void editor::displayMenu() // display menu and instruction
{
	system("CLS");
	system("color 70");
	cout << "\n ->Meun : use up/down arrow and Enter to navigate the page(press Esc to exit meun)";
	cout << "\n\n    -> Show message       " << menu[0] << "\n   -> Show word count    " << menu[1];
	cout << "\n    -> Clear              " << menu[2] << "\n   -> Format             " << menu[3];
	cout << "\n\n     Instructions:" << endl;
	cout << "    -> The arrow keys moves the cursor up,down, right and left." << endl;
	cout << "    -> Enter starts a new line and backspace deletes a character." << endl;
	cout << "    -> To copy, select a location where you want to begin, " << endl;
	cout << "	then press *F3* and move to another location where you " << endl;
	cout << "	would like to end the *copying* \n -> Press F3 again." << endl;
	cout << "    -> To paste simply press *F4* to paste at any location you want." << endl;
	cout << "    -> *F1* is to save text as txt file \n  *F2* to load text from txt file" << endl;
	cout << "    -> You can change setting at menu page and do specal function\n";
	cout << "    -> Clear function start a new page \n Format delete empty line from text";
	gotoxy(23, p + 4);
}
void editor::showMenu()
{
	char inp = ' ';
	p = 0; // cursor index for user
	menu[3] = menu[2] = "OK";
	displayMenu();
	while ((inp = _getch()) != 27) // only allow user to input up, down, enter, and esc
	{
		switch (inp)
		{
		case -32:
		{
			switch (_getch())
			{
			case 72: // when user input up
			{
				if (p == 0)
					break;
				else
					p--;
			}
			break;
			case 80: // when user press down
			{
				if (p == 3)
					break;
				else
					p++;
			}
			break;
			}
		}
		break;
		case 13:
		{ // change setting depending on what the user input
			if (menu[p] == "OFF")
				menu[p] = "ON";
			else if (menu[p] == "ON")
				menu[p] = "OFF";
			else if (menu[p] == "OK")
				menu[p] = "DONE";

			if (menu[3] == "DONE")
				format();
			if (menu[2] == "DONE")
				clear();
		}
		break;
		}
		displayMenu();
	}
}
void editor::format()
{
	for (auto it = line.begin(); it != line.end();)
	{
		node* currentNode = *it;

		int characterCount = 0;
		while (currentNode->next->next != nullptr)
		{
			if (currentNode->next->c != '\n')
			{
				characterCount++;
			}
			currentNode = currentNode->next;
		}

		// Check if the line has only one visible character
		if (characterCount == 1)
		{
			it = line.erase(it); // erase the element and get the iterator to the next element
			y--;
		}
		else
		{
			++it; // move to the next element
		}
	}
}


void editor::text() // load normal character
{
	t = new node;
	t->c = input;
	s->prev->next = t;
	t->prev = s->prev;
	s->prev = t;
	t->next = s;
	x++;
}
void editor::viewStoredFileNames()
{
	system("CLS");
	cout << "List of Stored Files:\n";

	for (const string& fileName : fileList)
	{
		cout << "->" << fileName << endl;
	}

	_getch(); // Wait for user input before clearing the screen
}

void editor::run()
{
	while ((input = _getch()) != 27) // take user input and exit out if esc is inputed
	{
		switch (input)
		{
		case -32:
			arrowKey(); // function for arrow key
			break;
		case 8:
			backspace(); // function for backspace
			break;
		case 13:
			enter(); // function ener
			break;
		case 0:
			fKey(); // function for special key
			break;
		case 'x': // view  List of stored File name
			viewStoredFileNames();
			break;
		case 26: // Ctrl + Z (undo)

			undo();
			break;

		case 25: // Ctrl + Y (redo)

			redo();
			break;
		default:
			text(); // function for normal input
		}
		display();
	}
}
void editor::clear()
{
	deleteNode(); // delete current text
	x = 0, y = 0; // initialize variable for first line
	s = new node, f = new node, t = new node;
	f->prev = t->next = f31 = f32 = nullptr;
	f->next = t->prev = s; // initialize variable for first line
	s->prev = f;
	s->next = t;
	s->c = ' ';		   // initialize variable for first line
	line.push_back(f); // initialize first element
}
void editor::display() // display board
{
	string temp = ""; // temp string that stores the text for each line
	system("CLS");
	system("color 50");
	int c = 0; // word count
	cout << " Press F6 to go to menu and instructions, Esc to exit\n\n     ______________________________________________________________________ \n";

	for (const auto& lineNode : line) // Iterate through each line
	{
		node* ptr = lineNode; // Set ptr to the beginning of the line

		while (ptr != nullptr && ptr->next != nullptr && ptr->next->next != nullptr) // Check for null pointers
		{
			temp += ptr->next->c;
			ptr = ptr->next;
			c++;
		}

		cout << "     |" << left << setw(68) << temp << "|\n"; // display the line
		temp = "";
	}

	cout << "     |                                                                    |"
		<< "\n     |                                                                    |"
		<< "\n     ----------------------------------------------------------------------\n"; // display the rest of the box

	if (menu[1] == "ON")
		cout << "     Word Count: " << c << " letters " << endl; // display word count if the user selects that option

	if (menu[0] == "ON")
		cout << "\n     Message:\n     " << message << endl << endl; // display message count if the user selects that option

	message = "no Message";
	gotoxy(x + 6, y + 3);
}
void editor:: undo()
{
	if (s != nullptr)
	{
		str.push_back(s->c);
		if (s->prev != f)
		{
			auto temp = s->prev;
			s->prev = temp->prev;
			s->prev->next = s;
			deleteWords.push_back(temp->c); // Keep track of the removed character
			delete temp;
			x--;
		}
	}
}
void editor:: redo()
{
	if (!deleteWords.empty())
	{
		auto newNode = new node;
		newNode->c = deleteWords.back();
		s->prev->next = newNode;
		newNode->prev = s->prev;
		newNode->next = s;
		s->prev = newNode;
		deleteWords.pop_back();
		x++;
	}
}

int main()
{
	editor d;
	d.fileList = d.loadFileList();
	d.run(); // run editor
	return 0;
}