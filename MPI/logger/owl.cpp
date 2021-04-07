#define _CRT_SECURE_NO_WARNINGS
#include "Owl.hpp" 

Owl owlInstance;

Owl::Owl()
{
	this->b_init = false;
}

Owl::~Owl()
{
	if (this->b_init) this->shutdown();
}


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}


bool Owl::init(const OwlSettings& settings)
{
	this->settings = settings;
	if (!this->b_init)
	{
		if (this->settings.b_overwriteFile)
		{
			this->ofs.open(this->settings.path + this->settings.filename);
		}
		else
		{
			this->ofs.open(this->settings.path + currentDateTime() + "_" + this->settings.filename);
		}

		if (this->ofs.is_open())
		{
			this->b_init = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void Owl::shutdown()
{
	if (this->b_init)
	{
		this->ofs.close();
		this->b_init = false;
	}
}

std::string Owl::prefix(const std::string& file, const int line)
{
	std::string result = "";
	if (this->settings.b_datetime)
	{
		result += currentDateTime();

		if (!this->settings.b_fileline) result += ": ";
		else result += ", ";
	}
	if (this->settings.b_fileline)
	{
		static char buffer[33];
		char* intStr = _itoa(line, buffer, 10);

		result += file + "(" + std::string(buffer) + "): ";
	}
	return result;
}