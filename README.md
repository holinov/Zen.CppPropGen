Zen.CppPropGen
==============

C++ property generator.
Could be used to generate or update property getters and setters it uses /*--prop fields--*/ and /*--prop methods--*/ as makrs to insert and update fields and accessors in class.

Generated example:
```C++
class GeneratedClass
{
public:
	GeneratedClass(){};
	~GeneratedClass(){};

	/*--prop methods--*/
	const string get_resultPath(){
		return m_resultPath;
	}
	void set_resultPath(string val){
		m_resultPath = val;
	}

	const string get_templatePath(){
		return m_templatePath;
	}
	void set_templatePath(string val){
		m_templatePath = val;
	}
	/*--prop methods--*/

private:
	/*--prop fields--*/
	string m_resultPath;
	string m_templatePath;
	/*--prop fields--*/
};
```
