#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <iterator>
#include <iomanip>

using namespace std;

struct citylocation
{
	int xAxis;
	int yAxis;

	string cityName;
	int cityCode;
	
};

struct cloudcover
{
	int xAxis;
	int yAxis;

	int cloudValue;
};

struct pressure
{
	int xAxis;
	int yAxis;

	int pressureValue;
};

// global variables and vector declaration
int xLower;
int xUpper;
int yLower;
int yUpper;

vector <citylocation> *cl_vector  = new vector <citylocation>;
vector <cloudcover> *cc_vector = new vector <cloudcover>;
vector <pressure> *pr_vector = new vector <pressure>;
vector <citylocation> *unique_cl_vector = new vector <citylocation>;
vector <vector <citylocation> > new2DVector;
vector <vector <citylocation> > new2DVector_2;
vector <double> temp_cc_vector;
vector <double> temp_pr_vector;	

// function prototypes
bool cityCodeOperator (citylocation &cl_struct1, citylocation &cl_struct2);
void sortCityCode ();
void createUniqueCityVector(vector <citylocation> &unique_cl_vector);
bool removeSameCityCode (citylocation &cl_struct1, citylocation &cl_struct2);
bool sortVectorOperator(citylocation &cl_struct1, citylocation &cl_struct2);
bool sortXYOperator (citylocation &cl_struct1, citylocation &cl_struct2);
void printSummary();

void resetScreen();
int mainMenu();
void returnMainMenu();
void readRange (string, bool);
void readFile (string, int, vector <citylocation> &cl_vector, vector <cloudcover> &cc_vector, vector <pressure> &pr_vector);
void displayCityMap();
void displayCloudinessIdx();
void displayCloudLMH();
void displayPressureIdx();
void displayPressureLMH();
void processChoice(int);
void processConfig();
// end of prototype

// main method to call out the main menu function
int main ()
{ 	
	resetScreen();
	processChoice(mainMenu()); 
	
	return(0);
} // end main() ...

// reset the screen
void resetScreen()
{
	system("clear");
} // end resetScreen() ...

// main menu returns int of the option selected to be passed as argument in processChoice()
int mainMenu()
{	
	int choice;

	cout << "Student ID 	: " << "4603229" << endl;
	cout << "Student Name 	: " << "SALLY" << endl;
	cout << "--------------------------------------" << endl;
	cout << "Welcome to Weather Information Processing System!" << endl;
	cout << endl;
	cout << "1) 	Read in and process a configuration file" << endl;
	cout << "2) 	Display city map" << endl;
	cout << "3) 	Display cloud coverage map (cloudiness index)" << endl;
	cout << "4) 	Display cloud coverage map (LMH symbols)" << endl;
	cout << "5) 	Display atmospheric pressure map (pressure index)" << endl;
	cout << "6) 	Display atmospheric pressure map (LMH symbols)" << endl;
	cout << "7) 	Show weather forecast summary report" << endl;
	cout << "8) 	Quit" << endl;
	cout << endl;
    cout << "Please enter your choice: ";

    cin >> choice;

    // validate user input is in range
	while (cin.fail() || choice < 1 || choice > 8) 
	{
		cout << "Please enter a valid choice: ";
		cin.clear();
		cin.ignore(512, '\n');
		cin >> choice;
	}

    return choice;
} // end mainMenu() ...

//Returns user to main menu after showing selected option
void returnMainMenu()
{
	char enter = 'a';
	
	cout << endl;
	
	while (enter != '\n') 
    {
        cout << "Press <enter> to go back to main menu";
        cin.get(enter);
        cin.ignore();
        cin.clear();
    }
    
    cout << "\n\n\n";
	
	resetScreen();
	processChoice(mainMenu());
} // end returnMainMenu()

//Reads in the IdxRange of X and Y
void readRange (string sentence, bool isXAxis)
{
    /*
    -- any symbol any number of times followed by first appearance of X to Y
    -- any symbol any number of times followed by "=" followed by 
    -- first appearance of digit any number of times followed by 
    -- "-" followed by
    -- first appearance of digit any number of times
    -- match X=0-8 or Y=0-8
    -- There are 4 capturing groups here, digits are in [3] and [4]
    */
    regex integers(R"(.*([X-Y]).*(=(\d*)-(\d*)))");
    smatch match;
   
	if (regex_search(sentence, match, integers))
    {
        if (isXAxis)
		{
			xLower = stoi(match[3]);
			xUpper = stoi(match[4]);
			cout << "Reading in GridX_IdxRange: " << xLower << "-" << xUpper << " ... done!" << endl;
		}
		else
		{
			yLower = stoi(match[3]);
			yUpper = stoi(match[4]);
			cout << "Reading in GridY_IdxRange: " << yLower << "-" << yUpper << " ... done!" << endl;
		}
	}	
} // end readRange() ...

// open and read in the 3 external files data into struct vectors
void readFile (string filename, int counter, vector <citylocation> &cl_vector, vector <cloudcover> &cc_vector, vector <pressure> &pr_vector)
{
	fstream inputFile;
	string aLine;

    // struct variables
	citylocation cl1;
	cloudcover cc2;
	pressure pr3;

	switch (counter)
	{
		case 1:
			inputFile.open(filename.c_str(), fstream::in);

			// store data into the vector of struct 'citylocation'
			while (getline (inputFile, aLine))
			{
				regex integers(R"(\[(\d*), (\d*)]-(\d*)-(\w+(\s*\w*)+))");
				smatch match;
				
   				if (regex_search(aLine, match, integers))
   				{
					cl1.xAxis = stoi(match[1]);
					cl1.yAxis = stoi(match[2]);
					cl1.cityCode = stoi(match[3]);
					cl1.cityName = (match[4]);

                    // add cl1 to cl_vector
					cl_vector.push_back(cl1);
				}
			}
			break;

		case 2:
			inputFile.open(filename.c_str(), fstream::in);
			// store data into the vector of struct 'cloudcover'
			while (getline (inputFile, aLine))
			{
				regex integers(R"(\[(\d*), (\d*)]-(\d*))");
				smatch match;
				
   				if (regex_search(aLine, match, integers))
   				{
					cc2.xAxis = stoi(match[1]);
					cc2.yAxis = stoi(match[2]);
					cc2.cloudValue = stoi(match[3]);

                    // add cc2 to cc_vector
					cc_vector.push_back(cc2);
				}
			}
			break;

		case 3:
			inputFile.open(filename.c_str(), fstream::in);
			// store data into the vector of struct 'pressure'
			while (getline (inputFile, aLine))
			{
				regex integers(R"(\[(\d*), (\d*)]-(\d*))");
				smatch match;
				
   				if (regex_search(aLine, match, integers))
   				{
					pr3.xAxis = stoi(match[1]);
					pr3.yAxis = stoi(match[2]);
					pr3.pressureValue = stoi(match[3]);

					pr_vector.push_back(pr3);
				}
			}
			break;
	}

    // close the file
	inputFile.close();
	
	cout << filename << " ... data successfully stored!" << endl;
} // end readFile() ...


// option 2, display city map
void displayCityMap()
{
	bool test;
	resetScreen();
	cout << endl << "Displaying City Map" << endl << endl;
	
    // total printing 11 rows
	for(int row = 0; row <= yUpper + 3; row++)
	{
        // row 0 or row 10th
    	if (row == 0 || row == yUpper + 2)
    	{
            // total printing 11 columns
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
                // row 0 column 0 or row 10th column 0
    			if (column == 0)
    			{
    				cout << setw(3) << " ";
    			}
                // all other columns of row 0 and row 10th
    			else 
    			{
    				cout << setw(3) << "#";
    			}
    		}
    	}
        // row 1-9
    	else if (row >= 1 && row <= yUpper + 1)
    	{
            // total printing 11 columns
    		for (int column = 0; column <= xUpper + 3; column++)
    		{    	
    		    test = false;		
    		    
                // row 1 onwards, if column 0
    			if (column == 0)
    			{
    				cout << setw(3) << yUpper + 1 - row;
    			}
                // row 1 onwards, if column 1 or column 11
    			else if (column == 1 || column == xUpper + 3)
    			{
    				cout << setw(3) << "#";
    			}
                // row 1 onwards, if column 2-10
    			else 
    			{
                    // cl_vector.size() = 14	
					// each column loop 14 times to check all the coordinates in the cl_vector
    				for (int cord_idx = 0; cord_idx < cl_vector->size(); cord_idx++)
    				{	
						// accessing citylocation vector via its index.xAxis and index.yAxis
						// for eg cl_vector[0]  is [1,1]
						//		  cl_vector[1]  is [1,2]
						//		  cl_vector[2]  is [1,3]
						//		  cl_vector[3]  is [2,1]
						//		  cl_vector[4]  is [2,2]
						//		  cl_vector[5]  is [2,3]
						//		  cl_vector[6]  is [2,7]
						// 		  cl_vector[7]  is [2,8]
						//		  cl_vector[8]  is [3,1]
						//		  cl_vector[9]  is [3,2]
						//		  cl_vector[10] is [3,3]
						//		  cl_vector[11] is [3,7]
						//		  cl_vector[12] is [3,8]
						//		  cl_vector[13] is [7,7]
						if(cl_vector->at(cord_idx).xAxis == column - 2 && cl_vector->at(cord_idx).yAxis == yUpper + 1 - row)
	 					{
	 						cout << setw(3) << cl_vector->at(cord_idx).cityCode;
	 						test = true;
	 					}
	 				}
	 				
		 			if (test == false)
		 			{
		 				cout << setw(3) << " ";
		 			}
    			}
    		}
    	}
		// row 11
    	else
    	{
    		cout << setw(6) << " ";
    		for (int column = 0; column <= xUpper; column++)
    		{
    			cout << setw(3) << column;
    		}
    	}
    	cout << endl;
    }

    returnMainMenu();
} // end displayCityMap() ...


//Option 3, displaying cloud index map
void displayCloudinessIdx()
{
	bool test;
	resetScreen();
	cout << endl << "Displaying Cloud Coverage Map (Cloudiness Index)" << endl << endl;
	
	// print 11 rows
	for(int row = 0; row <= yUpper + 3; row++)
	{
		// if row 0 or row 10
    	if (row == 0 || row == yUpper + 2)
    	{
			// for row 0 col 0 - 11 or row 10 col 0-11
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
				// if row 0 col 0 or row 10 col 0
    			if (column == 0)
    			{
    				cout << setw(3) << " ";
    			}
				// else row 0 col 1 - 11 or row 10 col 1 - 11
    			else 
    			{
    				cout << setw(3) << "#";
    			}
    		}
    	}
		// if row 1 - 9
    	else if (row >= 1 && row <= yUpper + 1)
    	{
			// for col 0 - 11
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
    			test = false;
    			
				// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << yUpper + 1 - row;
    			}
				// if col 1 or col 11
    			else if (column == 1 || column == xUpper + 3)
    			{
    				cout << setw(3) << "#";
    			}
				// col 2 - 10
    			else 
    			{
					// cc_vector.size() = 81, so each column loops 81 times		
    				for (int cord_idx = 0; cord_idx < cc_vector->size(); cord_idx++)
    				{
						// check col 2 - 10
						// if coordinates of cc_vector matches, print.
						if(cc_vector->at(cord_idx).xAxis == column - 2 && cc_vector->at(cord_idx).yAxis == yUpper + 1 - row)
	 					{
	 						if (cc_vector->at(cord_idx).cloudValue >= 0 && cc_vector->at(cord_idx).cloudValue < 10)
	 						{
	 							cout << setw(3) << "0";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 10 && cc_vector->at(cord_idx).cloudValue < 20)
	 						{
	 							cout << setw(3) << "1";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 20 && cc_vector->at(cord_idx).cloudValue < 30)
	 						{
	 							cout << setw(3) << "2";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 30 && cc_vector->at(cord_idx).cloudValue < 40)
	 						{
	 							cout << setw(3) << "3";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 40 && cc_vector->at(cord_idx).cloudValue < 50)
	 						{
	 							cout << setw(3) << "4";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 50 && cc_vector->at(cord_idx).cloudValue < 60)
	 						{
	 							cout << setw(3) << "5";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 60 && cc_vector->at(cord_idx).cloudValue < 70)
	 						{
	 							cout << setw(3) << "6";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 70 && cc_vector->at(cord_idx).cloudValue < 80)
	 						{
	 							cout << setw(3) << "7";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 80 && cc_vector->at(cord_idx).cloudValue < 90)
	 						{
	 							cout << setw(3) << "8";
	 						}
	 						else
	 						{
	 							cout << setw(3) << "9";
	 						}
	 						
	 						test = true;
	 					}
	 				}
	 				// if coordinates of cc_vector don't match, do this	 					
	 				if (test == false)
		 			{
		 				cout << setw(3) << " ";
		 			}
    			}
    		}
    	}
		// row 11 col 0 - 10
    	else
    	{
    		cout << setw(6) << " ";
    		for (int column = 0; column <= xUpper; column++)
    		{
    			cout << setw(3) << column;
    		}
    	}
    	
    	cout << endl;
    }

    returnMainMenu();
} // end displayCloudinessIdx() ...


// option 4, displaying cloud LMH map
void displayCloudLMH()
{
	bool test;
	resetScreen();
	cout << endl << "Displaying Cloud Coverage Map (LMH symbols)" << endl << endl;
	
	// print 11 rows
	for(int row = 0; row <= yUpper + 3; row++)
	{
		// if row 0 or row 10
    	if (row == 0 || row == yUpper + 2)
    	{
			// for row 0 col 0 - 11 or row 10 col 0-11
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
				// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << " ";
    			}
				// if col 1 - 11
    			else 
    			{
    				cout << setw(3) << "#";
    			}
    		}
    	}
		// if row 1-9
    	else if (row >= 1 && row <= yUpper + 1)
    	{
			// for col 0-11
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
    			test = false;
    			
				// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << yUpper + 1 - row;
    			}
				// if col 1 or 11
    			else if (column == 1 || column == xUpper + 3)
    			{
    				cout << setw(3) << "#";
    			}
				// if col 2 - 10
    			else 
    			{
					// cc_vector.size() = 81, so each column loops 81 times		
    				for (int cord_idx = 0; cord_idx < cc_vector->size(); cord_idx++)
    				{
						if(cc_vector->at(cord_idx).xAxis == column - 2 && cc_vector->at(cord_idx).yAxis == yUpper + 1 - row)
	 					{
	 						if (cc_vector->at(cord_idx).cloudValue >= 0 && cc_vector->at(cord_idx).cloudValue < 35)
	 						{
	 							cout << setw(3) << "L";
	 						}
	 						else if (cc_vector->at(cord_idx).cloudValue >= 35 && cc_vector->at(cord_idx).cloudValue < 65)
	 						{
	 							cout << setw(3) << "M";
	 						}
	 						else
	 						{
	 							cout << setw(3) << "H";
	 						}

	 						test = true;
	 					}
	 				}
	 				
		 			if (test == false)
		 			{
		 				cout << setw(3) << " ";
		 			}
    			}
    		}
    	}
		// row 11 col 0 - 10
    	else
    	{
    		cout << setw(6) << " ";
    		for (int column = 0; column <= xUpper; column++)
    		{
    			cout << setw(3) << column;
    		}
    	}
    	
    	cout << endl;
    }
    
    returnMainMenu();
} // end displayCloudLMH() ...

//Option 5, displaying pressure index map
void displayPressureIdx()
{
	bool test;
	resetScreen();
	cout << endl << "Displaying Atmospheric Pressure Map (Pressure Index)" << endl << endl;
	
	// print 11 rows
	for(int row = 0; row <= yUpper + 3; row++)
	{
		// if row 0 or row 10
    	if (row == 0 || row == yUpper + 2)
    	{
			// for (row 0 col 0 - 11) or (row 10 col 0 - 11)
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
				// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << " ";
    			}
				// if col 1 - 11
    			else 
    			{
    				cout << setw(3) << "#";
    			}
    		}
    	}
		// if row 1 - 9
    	else if (row >= 1 && row <= yUpper + 1)
    	{
			// for col 0 - 11
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
    			test = false;
    			// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << yUpper + 1 - row;
    			}
				// if col 1 or col 11
    			else if (column == 1 || column == xUpper + 3)
    			{
    				cout << setw(3) << "#";
    			}
    			else 
				// if col 2 - 10
    			{
					// pr_vector.size() = 81, so each column loops 81 times			
    				for (int cord_idx = 0; cord_idx < pr_vector->size(); cord_idx++)
    				{
						if(pr_vector->at(cord_idx).xAxis == column - 2 && pr_vector->at(cord_idx).yAxis == yUpper + 1 - row)
	 					{
	 						if (pr_vector->at(cord_idx).pressureValue >= 0 && pr_vector->at(cord_idx).pressureValue < 10)
	 						{
	 							cout << setw(3) << "0";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 10 && pr_vector->at(cord_idx).pressureValue < 20)
	 						{
	 							cout << setw(3) << "1";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 20 && pr_vector->at(cord_idx).pressureValue < 30)
	 						{
	 							cout << setw(3) << "2";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 30 && pr_vector->at(cord_idx).pressureValue < 40)
	 						{
	 							cout << setw(3) << "3";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 40 && pr_vector->at(cord_idx).pressureValue < 50)
	 						{
	 							cout << setw(3) << "4";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 50 && pr_vector->at(cord_idx).pressureValue < 60)
	 						{
	 							cout << setw(3) << "5";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 60 && pr_vector->at(cord_idx).pressureValue < 70)
	 						{
	 							cout << setw(3) << "6";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 70 && pr_vector->at(cord_idx).pressureValue < 80)
	 						{
	 							cout << setw(3) << "7";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 80 && pr_vector->at(cord_idx).pressureValue < 90)
	 						{
	 							cout << setw(3) << "8";
	 						}
	 						else
	 						{
	 							cout << setw(3) << "9";
	 						}
	 						
	 						test = true;
	 					}
	 				}
	 				
	 				if (test == false)
		 			{
		 				cout << setw(3) << " ";
		 			}
    			}
    		}
    	}
		// if row 11 col 0 - 10
    	else
    	{
    		cout << setw(6) << " ";
    		for (int column = 0; column <= xUpper; column++)
    		{
    			cout << setw(3) << column;
    		}
    	}
    	
    	cout << endl;
    }
    
    returnMainMenu();
} // end displayPressureIdx() ...


// option 6, displaying pressure LMH map
void displayPressureLMH()
{
	bool test;
	resetScreen();
	cout << endl << "Displaying Atmospheric Pressure Map (LMH symbols)" << endl << endl;
	
	// print 11 rows
	for(int row = 0; row <= yUpper + 3; row++)
	{
		// if row 0 or row 10
    	if (row == 0 || row == yUpper + 2)
    	{
			// for (row 0 col 0 - 11) or (row 10 col 0 - 11)
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
				// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << " ";
    			}
				// if col 1 - 11
    			else 
    			{
    				cout << setw(3) << "#";
    			}
    		}
    	}

		// if row 1 - 9
    	else if (row >= 1 && row <= yUpper + 1)
    	{
			// for col 0 - 11
    		for (int column = 0; column <= xUpper + 3; column++)
    		{
    			test = false;
    			
				// if col 0
    			if (column == 0)
    			{
    				cout << setw(3) << yUpper + 1 - row;
    			}
				// if col 1 or col 11
    			else if (column == 1 || column == xUpper + 3)
    			{
    				cout << setw(3) << "#";
    			}
				// if col 2 - 10
    			else 
    			{		
    				for (int cord_idx = 0; cord_idx < pr_vector->size(); cord_idx++)
    				{
						if(pr_vector->at(cord_idx).xAxis == column - 2 && pr_vector->at(cord_idx).yAxis == yUpper + 1 - row)
	 					{
	 						if (pr_vector->at(cord_idx).pressureValue >= 0 && pr_vector->at(cord_idx).pressureValue < 35)
	 						{
	 							cout << setw(3) << "L";
	 						}
	 						else if (pr_vector->at(cord_idx).pressureValue >= 35 && pr_vector->at(cord_idx).pressureValue < 65)
	 						{
	 							cout << setw(3) << "M";
	 						}
	 						else
	 						{
	 							cout << setw(3) << "H";
	 						}
	 							 					
	 						test = true;
	 					}
	 				}
	 				if (test == false)
			 		{
			 			cout << setw(3) << " ";
			 		}
    			}
    		}
    	}
		// if row 11 col 0 - 10
    	else
    	{
    		cout << setw(6) << " ";
    		for (int column = 0; column <= xUpper; column++)
    		{
    			cout << setw(3) << column;
    		}
    	}
    	
    	cout << endl;
    }
    returnMainMenu();
} // end displayPressureLMH() ...


// sort in ascending order based on cityCode of cl_vector
bool cityCodeOperator (citylocation &cl_struct1, citylocation &cl_struct2)
{
	return (cl_struct1.cityCode < cl_struct2.cityCode);
} // end cityCodeOperator() ...


// sort citylocation vector cl_vector in ascending order based on cityCode of cl_vector
void sortCityCode ()
{
	sort(cl_vector->begin(), cl_vector->end(), cityCodeOperator);
} // end sortCityCode() ...


// check if there's 2 or more struct with the same cityCode
bool removeSameCityCode (citylocation &cl_struct1, citylocation &cl_struct2)
{
	return (cl_struct1.cityCode == cl_struct2.cityCode);
} // end removeSameCityCode() ...


// get only the unique cityCodes from the struct vector of citylocation, 
// unique_cl_vector contains a sorted citylocation struct vector
void createUniqueCityVector (vector <citylocation> &unique_cl_vector)
{
	copy(cl_vector->begin(), cl_vector-> end(), back_inserter(unique_cl_vector));
	
	vector<citylocation>::iterator it;
	it = unique(unique_cl_vector.begin(), unique_cl_vector.end(), removeSameCityCode);
	unique_cl_vector.resize(distance(unique_cl_vector.begin(),it));
} // end createUniqueCityVector() ...


// sort X and Y coordinates in new2DVector_2 in ascending order
bool sortXYOperator (citylocation &cl_struct1, citylocation &cl_struct2)
{
    if(cl_struct1.xAxis < cl_struct2.xAxis)
    {
        return (true);
    }
    else if(cl_struct1.xAxis == cl_struct2.xAxis && cl_struct1.yAxis < cl_struct2.yAxis)
    {
        return (true);
    }
    return (false);
} // end sortXYOperator() ...

bool sortVectorOperator (citylocation &cl_struct1, citylocation &cl_struct2)
{
	if(cl_struct1.xAxis == cl_struct2.xAxis && cl_struct1.yAxis == cl_struct2.yAxis)
    {
        return (true);
    }
   	return (false);
} // end sortVectorOperator() ...

// create a 2D vector for the surrounding area
void create2DVector (vector <vector <citylocation> > &new2DVector, vector <vector <citylocation> > &new2DVector_2)
{
	citylocation cl_var1;
	
	for (int i = 0; i < unique_cl_vector->size(); i++)
	{
		new2DVector.push_back(vector<citylocation>());

		// store unique city location data to new2DVector
		for(int j = 0;  j < cl_vector->size(); j++)
		{
			if (cl_vector->at(j).cityCode == unique_cl_vector->at(i).cityCode)
			{
				cl_var1.xAxis = cl_vector->at(j).xAxis;
                cl_var1.yAxis = cl_vector->at(j).yAxis;
				cl_var1.cityName = cl_vector->at(j).cityName;
                cl_var1.cityCode = cl_vector->at(j).cityCode;
                
                new2DVector[i].push_back(cl_var1);
             }
		}
	}
	
	citylocation cl_var2;
	
	for (int i = 0; i < new2DVector.size(); i++)
	{
		new2DVector_2.push_back(vector <citylocation>());
		
		// surrounding grids
		for (int k = 0; k < new2DVector[i].size(); k++)
		{
			cl_var2.cityName = new2DVector[i][k].cityName;
			cl_var2.cityCode = new2DVector[i][k].cityCode;
			
			cl_var2.xAxis = new2DVector[i][k].xAxis;
           	cl_var2.yAxis = new2DVector[i][k].yAxis;
           	
            new2DVector_2[i].push_back(cl_var2);
        
            cl_var2.xAxis = new2DVector[i][k].xAxis - 1;
            
           	new2DVector_2[i].push_back(cl_var2);

           	cl_var2.yAxis = new2DVector[i][k].yAxis - 1;

          	new2DVector_2[i].push_back(cl_var2);
          	
            cl_var2.yAxis = new2DVector[i][k].yAxis + 1;

           	new2DVector_2[i].push_back(cl_var2);
            	
           	cl_var2.xAxis = new2DVector[i][k].xAxis;
            
            new2DVector_2[i].push_back(cl_var2);
            
           	cl_var2.yAxis = new2DVector[i][k].yAxis - 1;

            new2DVector_2[i].push_back(cl_var2);
            
            cl_var2.xAxis = new2DVector[i][k].xAxis + 1;

            new2DVector_2[i].push_back(cl_var2);
            
           	cl_var2.yAxis = new2DVector[i][k].yAxis;

           	new2DVector_2[i].push_back(cl_var2);
            
           	cl_var2.yAxis = new2DVector[i][k].yAxis + 1;

            new2DVector_2[i].push_back(cl_var2);
        }
    }
    
    vector<citylocation>::iterator it1;
    
    for (int i = 0; i < new2DVector_2.size(); i++) 
   	{

		sort(new2DVector_2[i].begin(), new2DVector_2[i].end(), sortXYOperator);
		
		for (int j = 0; j < new2DVector_2[i].size(); j++) 
		{
			it1 = unique(new2DVector_2[i].begin(), new2DVector_2[i].end(), sortVectorOperator);
			new2DVector_2[i].resize(distance(new2DVector_2[i].begin(),it1));
		}
	}
} // end create2DVector() ...

// option 7, display the summary report
void printSummary()
{
	resetScreen(); 

	// temporary and counter variables
	double temp = 0;
	double counter = 0;
	
	cout << endl << "Weather Forecast Summary Report" << endl;
    cout << "---------------------------------" << endl;
	
	
	for (int i = unique_cl_vector->size()-1; i >= 0; i--)
   	{
		for (int j = 0; j < new2DVector_2[i].size(); j++) 
		{
			for (int k = 0; k < cc_vector->size(); k++)
			{
				if (new2DVector_2[i][j].xAxis == cc_vector->at(k).xAxis && new2DVector_2[i][j].yAxis == cc_vector->at(k).yAxis)
				{
					temp = temp + cc_vector->at(k).cloudValue;
					counter++;
				}
			}
		}
		
		double ACC = temp/counter;
		temp_cc_vector.push_back(ACC);
		counter = 0.0;
		temp = 0.0;

		for (int j = 0; j < new2DVector_2[i].size(); j++) 
		{
			for (int k = 0; k < pr_vector->size(); k++)
			{
				if (new2DVector_2[i][j].xAxis == pr_vector->at(k).xAxis && new2DVector_2[i][j].yAxis == pr_vector->at(k).yAxis)
				{
					temp = temp + pr_vector->at(k).pressureValue;
					counter++;
				}
			}
		}
		
		double APR = temp/counter;
		temp_pr_vector.push_back(APR);
		counter = 0.0;
		temp = 0.0;

		cout << "City Name" << "\t" << ": " << unique_cl_vector->at(i).cityName << endl;
		cout << "City ID" << "\t\t" << ": " << unique_cl_vector->at(i).cityCode << endl;
		
		char cloudLMH;
		char pressureLMH;
		double probability;
		
		if (temp_cc_vector[unique_cl_vector->size()-1-i] < 35)
		{
			cloudLMH = 'L';
		}
		else if (temp_cc_vector[unique_cl_vector->size()-1-i] >= 35 && temp_cc_vector[unique_cl_vector->size()-1-i] < 65)
		{
			cloudLMH = 'M';
		}
		else
		{
			cloudLMH = 'H';
		}
		
		if (temp_pr_vector[unique_cl_vector->size()-1-i] < 35)
		{
			pressureLMH = 'L';
		}
		else if (temp_pr_vector[unique_cl_vector->size()-1-i] >= 35 && temp_pr_vector[unique_cl_vector->size()-1-i] < 65)
		{
			pressureLMH = 'M';
		}
		else
		{
			pressureLMH = 'H';
		}
		
		if (cloudLMH == 'H' && pressureLMH == 'L')
		{
			probability = 90.00;
		}
		else if (cloudLMH == 'M' && pressureLMH == 'L')
		{
			probability = 80.00;
		}
		else if (cloudLMH == 'L' && pressureLMH == 'L')
		{
			probability = 70.00;
		}
		else if (cloudLMH == 'H' && pressureLMH == 'M')
		{
			probability = 60.00;
		}
		else if (cloudLMH == 'M' && pressureLMH == 'M')
		{
			probability = 50.00;
		}
		else if (cloudLMH == 'L' && pressureLMH == 'M')
		{
			probability = 40.00;
		}
		else if (cloudLMH == 'H' && pressureLMH == 'H')
		{
			probability = 30.00;
		}
		else if (cloudLMH == 'M' && pressureLMH == 'H')
		{
			probability = 20.00;
		}
		else if (cloudLMH == 'L' && pressureLMH == 'H')
		{
			probability = 10.00;
		}
		
		cout << "Ave. Cloud Cover (ACC)" << "\t" << ": " << fixed << setprecision(2) << temp_cc_vector[unique_cl_vector->size()-1-i] << " (" << cloudLMH << ")" << endl;
		
		cout << "Ave. Pressure (AP)" << "\t" << ": " << fixed << setprecision(2) << temp_pr_vector[unique_cl_vector->size()-1-i] <<" (" << pressureLMH << ")" << endl;
		
		cout << "Probability of Rain (%)" << "\t" << ": " << fixed << setprecision(2) << probability << endl;
		
		if (probability == 10.00)
		{
			cout << "~" << endl << "~~" << endl;
		}
		else if (probability == 20.00)
		{
			cout << "~~" << endl << "~~~" << endl;
		}
		else if (probability == 30.00)
		{
			cout << "~~~" << endl << "~~~~" << endl;
		}
		else if (probability == 40.00)
		{
			cout << "~~~~" << endl << "~~~~~" << endl;
		}
		else if (probability == 50.00)
		{
			cout << "~~~~" << endl << "~~~~~" << endl << "    \\" << endl;
		}
		else if (probability == 60.00)
		{
			cout << "~~~~" << endl << "~~~~~" << endl << "   \\\\" << endl;
		}
		else if (probability == 70.00)
		{
			cout << "~~~~" << endl << "~~~~~" << endl << "  \\\\\\" << endl;
		}
		else if (probability == 80.00)
		{
			cout << "~~~~" << endl << "~~~~~" << endl << " \\\\\\\\" << endl;
		}
		else
		{
			cout << "~~~~" << endl << "~~~~~" << endl << "\\\\\\\\\\" << endl;
		}
		cout << endl;
	}
	
	returnMainMenu();
} // end printSummary() ...

// option from the main menu will be parsed into the switch to open the various functions
void processChoice(int choice)
{	
	switch(choice)
	{
		case 1: 
			processConfig(); 
			break;
		case 2: 
			displayCityMap();
			break;
		case 3: 
			displayCloudinessIdx();
			break;
		case 4:
			displayCloudLMH();
			break;
		case 5:
			displayPressureIdx();
			break;
		case 6:
			displayPressureLMH();
			break;
		case 7:
			printSummary();
			break;
		case 8:	// prevent memory leaks
			delete cl_vector;
			delete cc_vector;
			delete pr_vector;
			delete unique_cl_vector;
			cout << "Terminating Program ... Success!" << endl;
			exit(0);
			break;
	}
} // end processChoice() ...

// option 1, input configuration file
void processConfig()
{
	ifstream inData;
	string inputFileName;
	string aLine;
	size_t position;
	
	cout << "[Read in and process a configuration file]" << endl;
	cout << "Please enter config filename: ";
	
	cin >> inputFileName;
	
	cl_vector->clear();
	cc_vector->clear();
	pr_vector->clear();
	unique_cl_vector->clear();
	new2DVector.clear();
	new2DVector_2.clear();
	temp_cc_vector.clear();
	temp_pr_vector.clear();
	
	fstream inputFile;
	inputFile.open(inputFileName.c_str(), fstream::in);
	cout << endl;
	
	int counter = 0;

	// traverse line by line
    while (getline (inputFile, aLine))
    {
		position = aLine.find("GridX_IdxRange"); 
		
		// if X-Axis, call readRange()
    	if (position != string::npos)
    	{
    		readRange(aLine, true);
    	}
    	
    	position = aLine.find("GridY_IdxRange");    

		// if Y-Axis, call readRange()
    	if (position != string::npos)
    	{
    		readRange(aLine, false);
    	}
		
    	position = aLine.find(".txt");

		// if txt file, call readFile()
		if (position != string::npos)
		{
			counter++;
			
			if (counter == 1)
			{
				cout << endl << "Storing data from input file: " << endl;
			}
			readFile (aLine, counter, *cl_vector, *cc_vector, *pr_vector);
		}
    }
	
	inputFile.close();
	
	sortCityCode();
	createUniqueCityVector(*unique_cl_vector);
	create2DVector(new2DVector, new2DVector_2);
	
	if (counter > 0)
	{
		cout << endl << "All data successfully stored. Going back to main menu..." << endl;
	}
	else
	{
		cout << "No data has been found in \"" << inputFileName.c_str() << "\" Going back to main menu..." << endl;
	}
	cout << endl;
	
	processChoice(mainMenu()); 
} // end processConfig() ...