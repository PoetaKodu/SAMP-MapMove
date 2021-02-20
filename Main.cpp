/// MapMove by Soldier (aka CrossX)
/// Compile: g++ Main.cpp -o MapMove -std=c++17
/// ... or run "make"
/// Execute without arguments to print usage.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <string_view>
#include <functional>


constexpr std::string_view LookFor = "CreateDynamicObject(";


////////// Vector 3D ////////////////////////
struct Vec3 { double x, y, z; };

constexpr Vec3 operator+(Vec3 const& l, Vec3 const& r) {
	return { l.x + r.x, l.y + r.y, l.z + r.z };
}

constexpr Vec3 operator-(Vec3 const& l, Vec3 const& r) {
	return { l.x - r.x, l.y - r.y, l.z - r.z };
}

///////// Vector transform function signature
using Vec3TransformFn = std::function<void(Vec3&)>;

int findNthInString(std::string const& str_, char needle, int n_);

// Efficient file reading
std::string loadFileContent(std::string const& path_);

// Transforming objects in file
std::string transformFileContent(std::string content_, Vec3TransformFn transformFn_);

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		std::cerr 	<< "MapMove - Moves SAMP maps by certain delta in 3D world.\n"
					<< "Usage: MapMove <mapfile> <deltaX> <deltaY> <deltaZ>\n"
					<< "Example: MapMove map.txt -1000 50 100" << std::endl;
		return 0;
	}

	try {
		
		Vec3 delta = {
				std::stof(argv[2]),
				std::stof(argv[3]),
				std::stof(argv[4])
			};

		std::string inputFile = loadFileContent(argv[1]);
		auto transform = [&](Vec3& p) { p = p + delta; };

		std::ofstream{"moved.txt"} << transformFileContent(inputFile, transform);

	}
	catch(std::exception &exc) {
		std::cerr << "Error. An exception occurred, details: " << exc.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Error. An unknown exception occurred" << std::endl;
	}
}

/////////////////////////////////////////////////////
int findNthInString(std::string const& str_, char needle, int n_ = 1)
{
	int counter = 0;
  	for(size_t i = 0; i < str_.length(); ++i)
  	{
		if (str_[i] == needle)
    	{
			++counter;
			if (counter >= n_)
				return i;
    	}
  	}
  	return -1;
}

/////////////////////////////////////////////////////
std::string loadFileContent(std::string const& path_)
{
	std::string content;

	std::ifstream file(path_);
	if (file.is_open())
	{
		char buf[1024 * 1024];
		while(file.read(buf, sizeof buf))
			content.append(buf, file.gcount());

		content.append(buf, file.gcount());
	}

	return content;
}

/////////////////////////////////////////////////////
std::string transformFileContent(std::string content_, Vec3TransformFn transformFn_)
{
	std::string output;
	output.reserve(content_.length() * 2);

	std::istringstream iss(content_);

	std::string line;

	size_t lineIdx = 0;
	while(std::getline(iss, line))
	{
		++lineIdx;

		auto pos = line.find(LookFor);

		if (pos != std::string::npos)
		{
			std::string subLine = line.substr(pos + LookFor.length());

			Vec3 loc;
			int modelIdx = 0;
			if (!std::sscanf(subLine.c_str(), "%d,%lf,%lf,%lf", &modelIdx, &loc.x, &loc.y, &loc.z))
			{
				std::cout << "Could not parse line " << lineIdx << "!" << std::endl;
				continue;
			}
			else
			{
				transformFn_(loc);
				output += LookFor;
				output += std::to_string(modelIdx); output += ", ";
				output += std::to_string(loc.x); 	output += ", ";
				output += std::to_string(loc.y); 	output += ", ";
				output += std::to_string(loc.z);
				output += subLine.substr(findNthInString(subLine, ',', 4));
			}
		}
		else
		{
			// Append without changes
			output += line;
		}
		output += '\n';
	}

	return output;
}

