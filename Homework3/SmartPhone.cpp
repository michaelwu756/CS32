#include <iostream>
#include <string>
using namespace std;
class SmartPhone
{
	public:
		SmartPhone(string name);
		virtual ~SmartPhone();
		string name() const;
		virtual void printWebBrowser() const=0;
		virtual string futureMove() const=0;
	private:
		string m_name;
};
class Windows:public SmartPhone
{
	public:
		Windows(string name);
		~Windows();
		void printWebBrowser() const;
		string futureMove() const;
};
class iOS:public SmartPhone
{
	public:
		iOS(string name, string browser);
		~iOS();
		void printWebBrowser() const;
		string futureMove() const;
	private:
		string m_browser;
};
class Android:public SmartPhone
{
	public:
		Android(string name);
		~Android();
		void printWebBrowser() const;
		string futureMove() const;
};

SmartPhone::SmartPhone(string name):m_name(name){}
SmartPhone::~SmartPhone(){}
string SmartPhone::name() const
{
	return m_name;
}

Windows::Windows(string name):SmartPhone(name){}
Windows::~Windows()
{
	cout<<"Destroying "<<name()<<" the Windows"<<endl;
}
void Windows::printWebBrowser() const
{
	cout<<"IE";
}
string Windows::futureMove() const
{
	return "tries to maintain its market share";
}

iOS::iOS(string name,string browser):SmartPhone(name),m_browser(browser){}
iOS::~iOS()
{
	cout<<"Destroying "<<name()<<" the iOS"<<endl;
}
void iOS::printWebBrowser() const
{
	cout<<"Safari version "<<m_browser;
}
string iOS::futureMove() const
{
	return "continues to lose market share";
}

Android::Android(string name):SmartPhone(name){}
Android::~Android()
{
	cout<<"Destroying "<<name()<<" the Android"<<endl;
}
void Android::printWebBrowser() const
{
	cout<<"Chrome";
}
string Android::futureMove() const
{
	return "tries to gain market share";
}

void availability(const SmartPhone* cp)
{
    cout << cp->name() << ", using ";
    cp->printWebBrowser();
    cout << ", " << cp->futureMove() << "." << endl;
}

int main()
{
    SmartPhone* phones[4];
    phones[0] = new Windows("Alcatel OneTouch Fierce XL with Windows 10");
        // iOS has a name and initial version number for the Safari Browser 
    phones[1] = new iOS("iPhone 6 Plus", "10.0 beta 2");
    phones[2] = new Android("Samsung Galaxy S7");
    phones[3] = new Android("HTC 10");

    cout << "The SmartPhones availability!" << endl;
    for (int k = 0; k < 4; k++)
        availability(phones[k]);

        // Clean up the SmartPhones before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++)
	delete phones[k];
}