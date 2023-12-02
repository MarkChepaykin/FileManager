#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager {
public:
    FileManager(const std::string& rootDirectory) : rootDirectory_(rootDirectory) {}

    // Метод для отображения содержимого диска
    void ShowDiskContent() {
        for (const auto& entry : fs::directory_iterator(rootDirectory_)) {
            std::cout << entry.path() << std::endl;
        }
    }

    // Метод для создания папки
    bool CreateFolder(const std::string& folderName) {
        fs::path folderPath = rootDirectory_ / folderName;
        return fs::create_directory(folderPath);
    }

    // Метод для удаления папки или файла
    bool DeleteItem(const std::string& itemName) {
        fs::path itemPath = rootDirectory_ / itemName;
        return fs::remove_all(itemPath);
    }

    // Метод для переименования папки или файла
    std::error_code RenameItem(const std::string& oldName, const std::string& newName) {
        fs::path oldPath = rootDirectory_ / oldName;
        fs::path newPath = rootDirectory_ / newName;
        std::error_code ec; 
        fs::rename(oldPath, newPath, ec); 
        return ec; 
    }

    // Метод для копирования папки или файла
    std::error_code CopyItem(const std::string& sourceName, const std::string& destinationName) {
        fs::path sourcePath = rootDirectory_ / sourceName;
        fs::path destinationPath = rootDirectory_ / destinationName;
        std::error_code ec; 
        fs::copy(sourcePath, destinationPath, ec); 
        return ec; 
    }


    // Метод для вычисления размера папки или файла
    std::uintmax_t GetSize(const std::string& itemName) {
        fs::path itemPath = rootDirectory_ / itemName;
        return fs::file_size(itemPath);
    }


    // Метод для поиска файлов по маске включая подпапки
    void SearchFilesByMask(const std::string& mask) {
        for (const auto& entry : fs::recursive_directory_iterator(rootDirectory_)) {
            if (fs::is_regular_file(entry)) {
                std::string filename = entry.path().filename().string();
                if (EndsWith(filename, mask)) {
                    std::cout << entry.path() << std::endl;
                }
            }
        }
    }

    bool EndsWith(const std::string& str, const std::string& suffix) {
        if (str.length() >= suffix.length()) {
            return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
        }
        return false;
    }

    bool CompareStringsIgnoreCase(const std::string& str1, const std::string& str2) {
        return str1.size() == str2.size() && std::equal(str1.begin(), str1.end(), str2.begin(),
            [](char c1, char c2) { return std::tolower(c1) == std::tolower(c2); });
    }


private:
    fs::path rootDirectory_;
};



void DisplayMenu() {
    std::cout << "File Manager Menu:" << std::endl;
    std::cout << "1. Show Disk Content" << std::endl;
    std::cout << "2. Create Folder" << std::endl;
    std::cout << "3. Delete Folder/File" << std::endl;
    std::cout << "4. Rename Folder/File" << std::endl;
    std::cout << "5. Copy Folder/File" << std::endl;
    std::cout << "6. Get Size of Folder/File" << std::endl;
    std::cout << "7. Search Files by Mask" << std::endl;
    std::cout << "8. Exit" << std::endl;
}

int main() {
    FileManager fileManager("C:/Users/user/Desktop/learn/С++/FileManager");

    int choice;
    do {
        DisplayMenu();
        std::cout << "Enter your choice (1-8): ";
        std::cin >> choice;

        try {
            switch (choice) {
            case 1:
                fileManager.ShowDiskContent();
                break;
            case 2:
            {
                std::string folderName;
                std::cout << "Enter folder name to create: ";
                std::cin >> folderName;
                if (fileManager.CreateFolder(folderName)) {
                    std::cout << "Folder created successfully." << std::endl;
                }
                else {
                    std::cout << "Failed to create folder." << std::endl;
                }
            }
            break;
            case 3:
            {
                std::string itemName;
                std::cout << "Enter folder/file name to delete: ";
                std::cin >> itemName;
                if (fileManager.DeleteItem(itemName)) {
                    std::cout << "Item deleted successfully." << std::endl;
                }
                else {
                    std::cout << "Failed to delete item." << std::endl;
                }
            }
            break;
            case 4:
            {
                std::string oldName, newName;
                std::cout << "Enter old name: ";
                std::cin >> oldName;
                std::cout << "Enter new name: ";
                std::cin >> newName;
                std::error_code ec = fileManager.RenameItem(oldName, newName);
                if (!ec) {
                    std::cout << "Item renamed successfully." << std::endl;
                }
                else {
                    std::cout << "Failed to rename item. Error: " << ec.message() << std::endl;
                }
            }
            break;
            case 5:
            {
                std::string sourceName, destinationName;
                std::cout << "Enter source name: ";
                std::cin >> sourceName;
                std::cout << "Enter destination name: ";
                std::cin >> destinationName;
                std::error_code ec = fileManager.CopyItem(sourceName, destinationName);
                if (!ec) {
                    std::cout << "Item copied successfully." << std::endl;
                }
                else {
                    std::cout << "Failed to copy item. Error: " << ec.message() << std::endl;
                }
            }
            break;
            case 6:
            {
                std::string itemName;
                std::cout << "Enter folder/file name to get size: ";
                std::cin >> itemName;
                std::uintmax_t size = fileManager.GetSize(itemName);
                if (size != static_cast<std::uintmax_t>(-1)) {
                    std::cout << "Size of " << itemName << ": " << size << " bytes" << std::endl;
                }
                else {
                    std::cout << "Failed to get size of item." << std::endl;
                }
            }
            break;
            case 7:
            {
                std::string mask;
                std::cout << "Enter file mask (e.g., .txt): ";
                std::cin >> mask;
                fileManager.SearchFilesByMask(mask);
            }
            break;
            case 8:
                std::cout << "Exiting File Manager." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }
    } while (choice != 8);

    return 0;
}