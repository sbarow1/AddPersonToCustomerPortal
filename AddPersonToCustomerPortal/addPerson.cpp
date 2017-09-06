#include <iostream>
#include "webdriverxx.h"
#include <fstream>
#include <cstdlib>

using namespace webdriverxx;

int main() {

	std::string username;
	std::string password;
	std::cout << "Username? ";
	std::getline(std::cin, username);
	std::cout << "Password? ";
	std::getline(std::cin, password);

	WebDriver gc = Start(Chrome());
	gc
		.Navigate("https://cp.lattix.com/")
		.FindElement(ByCss("input#edit-name.form-text.required"))
		.SendKeys(username);
	gc
		.FindElement(ByCss("input#edit-pass.form-text.required"))
		.SendKeys(password)
		.Submit();

	auto people_element = [&] { return gc.FindElement(ByCss("a#toolbar-link-admin-people")); };
	Element element = WaitForValue(people_element);
	element.Click();

	auto frame_element = [&] { return gc.FindElement(ByCss("iframe.overlay-element.overlay-active")); };
	Element frame = WaitForValue(frame_element);
	gc.SetFocusToFrame(frame);

	Element addUser = gc.FindElement(ByLinkText("Add user"));
	addUser.Click();

	// Prompt for users email address
	std::string emailAddress;
	std::cout << "What is the email address?";
	std::cin >> emailAddress;

	gc.SetFocusToDefaultFrame();
	// Set focus to the new frame
	auto frame_addPerson = [&] { return gc.FindElement(ByCss("iframe.overlay-element.overlay-active")); };
	Element addPerson = WaitForValue(frame_addPerson);
	gc.SetFocusToFrame(addPerson);

	std::string html_source = gc.GetSource();
	std::ofstream fout("Add_person.txt");
	if (!fout.is_open())
	{
		std::cerr << "Could not open " << "Add_person.txt" << std::endl;
		exit(EXIT_FAILURE);
	}
	fout << html_source;

	// Input the addresses
	Element theName = gc.FindElement(ById("edit-name"));
	theName.SendKeys(emailAddress);
	Element theEmail = gc.FindElement(ByCss("input#edit-mail.form-text.required"));
	theEmail.SendKeys(emailAddress);
	Element password1 = gc.FindElement(ByCss("input#edit-pass-pass1.password-field.form-text.required.password-processed"));
	password1.SendKeys(emailAddress);
	Element password2 = gc.FindElement(ByCss("input#edit-pass-pass2.password-confirm.form-text.required"));
	password2.SendKeys(emailAddress);

	Element eval = gc.FindElement(ByCss("input#edit-roles-12.form-checkbox"));
	eval.Click();
	Element notify = gc.FindElement(ByCss("input#edit-notify.form-checkbox"));
	notify.Click();
	Element submit = gc.FindElement(ByCss("input#edit-submit.form-submit"));
	submit.Click();

	std::cin.get();
	std::cin.get();
	return 0;
}