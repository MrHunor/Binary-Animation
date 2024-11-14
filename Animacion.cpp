#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <Windows.h>
#pragma warning(disable : 4996)
using namespace std;

// Global variables
chrono::high_resolution_clock::time_point start;
string userinput;

// Function to log text to a file
bool LogIt(const string& Text) {
    ofstream FileHandler("Log.txt", ios::app); // Open in append mode
    if (FileHandler.is_open()) {
        FileHandler << Text;
        FileHandler.close(); // Close the file after writing
        return true;
    }
    else {
        return false;
    }
}

// Function to write the best times to a file
void writebesttime(const vector<double>& besttime) {
    ofstream FileHandler("BestTime.txt", ios::app);
    for (size_t i = 0; i < besttime.size(); i++) {
        FileHandler << besttime[i] << ",";
    }
    // Error handling (optional)
}
string getCurrentDateTime() {
    // Get the current time
    auto now = chrono::system_clock::now();

    // Convert to time_t to get calendar time (for date formatting)
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm now_tm = *localtime(&now_c);

    // Extract milliseconds
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // Format the date and time to a string
    ostringstream oss;
    oss << put_time(&now_tm, "%Y-%m-%d %H:%M:%S")   // Format date and time
        << '.' << setfill('0') << setw(3) << ms.count();  // Add milliseconds

    return oss.str();
}

// Function to read best times from a file
vector<double> readbesttime() {
    ifstream file("BestTime.txt");
    vector<double> numbers;

    if (file) {
        string line;
        if (getline(file, line)) {
            stringstream ss(line);
            string number;

            // Split the line by commas
            while (getline(ss, number, ',')) {
                // Convert each substring to a double and add it to the vector
                numbers.push_back(stod(number));
            }
        }
    }
    else {
        cerr << "Failed to open file" << endl;
        
        numbers = { 999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999 };
    }

    return numbers;
}

// Function to display best times
void displaybesttime(const vector<double>& Vector) {
    for (size_t i = 0; i < Vector.size(); i++) {
        cout << "Best time for " << i << " Bits is: " << Vector[i] << "s" << "\n";
    }
}

// Function to perform binary up-count animation
void binaryUpcountAnimation(int num_bits, int delay_ms) {
    int max_count = (1 << num_bits);  // Calculate 2^num_bits

    for (int i = 0; i < max_count; i++) {
        // Print each value as a binary number with leading zeros
        for (int j = num_bits - 1; j >= 0; j--) {
            cout << ((i >> j) & 1);
        }
        cout << "\n";

        // Delay between counts for animation
        Sleep(delay_ms);
    }
    cout << endl;  // Move to the next line after finishing the animation
}

int main() {
    string userinput;
    remove("Log.txt");
    LogIt(getCurrentDateTime());
    LogIt("\n");
    vector<double> Best_Time = readbesttime();
    remove("BestTime.txt");
    displaybesttime(Best_Time);
    cout << "How many bits should the program count?\n";
    cin >> userinput;

    // Loop through each bit length (from 1 to num_bits)
    for (int i = 0; i < stoi(userinput); i++) // Loop for each number of bits
    {
        start = chrono::high_resolution_clock::now();
        cout << "Trying for " << i << " Bits" << endl;
        binaryUpcountAnimation(i, 0);

        auto now = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration<double>(now - start);

        // Print elapsed time for the current bit length
        cout << "Counted up " << i << " Bits in " << elapsed.count() << " sec" << "\n";

        // Check if current time is faster or slower than the best time
        if (elapsed.count() > Best_Time[i]) {
            cout << "That was slower than the best time for " << i << " Bits, slower by "
                << elapsed.count() - Best_Time[i] << " sec.\n";
            LogIt("Attempt for " + to_string(i) + " Bits. That was "
                + to_string(elapsed.count() - Best_Time[i]) + " slower than Highscore.\n");
        }
        else if (elapsed.count() < Best_Time[i]) {
            cout << "That was faster than the best time for " << i << " Bits, faster by "
                << Best_Time[i] - elapsed.count() << " sec.\n";
            LogIt("Attempt for " + to_string(i) + " Bits. That was "
                + to_string(Best_Time[i] - elapsed.count()) + " faster than Highscore. Highscore was overwritten.\n");
            Best_Time[i] = elapsed.count(); // Update best time if current time is faster
        }
    }

    displaybesttime(Best_Time);
    writebesttime(Best_Time);
   
    system("pause");
    return 0;
}
