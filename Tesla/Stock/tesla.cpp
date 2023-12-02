#define _CRT_SECURE_NO_WARNINGS // Add this line to suppress warnings about unsafe functions
#include "csv.h"
#include <iostream>
#include <vector>
#include <string>

struct StockData {
    std::string date;
    double open, high, low, close, adjClose;
    long long volume; // Changed to long long to prevent data loss
};

std::vector<StockData> loadCsvData(const std::string& filePath) {
    std::vector<StockData> data;
    try {
        io::CSVReader<7, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> reader(filePath);
        reader.read_header(io::ignore_extra_column, "Date", "Open", "High", "Low", "Close", "Adj Close", "Volume");

        StockData stockData;
        while (reader.read_row(stockData.date, stockData.open, stockData.high, stockData.low,
            stockData.close, stockData.adjClose, stockData.volume)) {
            data.push_back(stockData);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred while reading CSV data: " << e.what() << std::endl;
        // Handle the exception, e.g., by returning an empty vector or rethrowing
    }
    return data;
}


bool isHeadAndShouldersBottom(const std::vector<StockData>& data, size_t index) {
    // Simple placeholder logic for illustration purposes only
    // This is not actual pattern detection logic
    if (index < 2 || index >= data.size() - 2) {
        return false;
    }
    // Check for a simple pattern where the middle day's low is lower than the lows of the days around it
    return data[index].low < data[index - 1].low&& data[index].low < data[index - 2].low&&
        data[index].low < data[index + 1].low&& data[index].low < data[index + 2].low;
}

bool isHeadAndShouldersTop(const std::vector<StockData>& data, size_t index) {
    // Simple placeholder logic for illustration purposes only
    // This is not actual pattern detection logic
    if (index < 2 || index >= data.size() - 2) {
        return false;
    }
    // Check for a simple pattern where the middle day's high is higher than the highs of the days around it
    return data[index].high > data[index - 1].high && data[index].high > data[index - 2].high &&
        data[index].high > data[index + 1].high && data[index].high > data[index + 2].high;
}





double simulateTrades(const std::vector<StockData>& data) {
    double portfolioValue = 1000.0;
    double shares = 0.0;
    bool holding = false;

    for (size_t i = 0; i < data.size() - 1; ++i) {
        if (!holding && isHeadAndShouldersBottom(data, i)) {
            // Buy at next day's opening price, not at closing price
            shares = portfolioValue / data[i + 1].open;
            portfolioValue = 0.0;
            holding = true;
        }
        else if (holding && isHeadAndShouldersTop(data, i)) {
            // Sell at next day's opening price
            portfolioValue = shares * data[i + 1].open;
            shares = 0.0;
            holding = false;
        }
    }

    // If still holding shares at the end, sell them at the last available close price
    if (holding) {
        portfolioValue = shares * data.back().close;
    }

    return portfolioValue;
}

int main() {
    std::string filePath = "D:/1_COde/C++/TSLA.csv";
    auto teslaData = loadCsvData(filePath);
    double finalPortfolioValue = simulateTrades(teslaData);
    std::cout << "Final Portfolio Value: " << finalPortfolioValue << std::endl;
    // Make sure all brackets are properly closed and semicolons are in place
    return 0;
}
