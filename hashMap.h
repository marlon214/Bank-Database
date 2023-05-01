/*
Name: Marlon Alejandro
NSHE: 5002573038
Assignment: 6
Description: Read in account information and allow access to users with the proper credentials
Input: Excel file that contains all account information and user inputs
Output: Options and account details
*/

#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

template <class t1, class t2>
class hashMap
{
public:
	hashMap(std::size_t = 5);
	t2& operator[](t1);
private:
	struct keyVal
	{
		keyVal()
		{
			key = t1();
			value = t2();
		}

		t1 key;
		t2 value;
	};

	void resize(std::size_t);


	std::size_t hash1(std::string);
	std::size_t hash2(std::string);

	std::vector<keyVal> table1;
	std::vector<keyVal> table2;

	std::size_t items1;
	std::size_t items2;
};

template <class t1, class t2>
hashMap<t1, t2>::hashMap(std::size_t size)
{
	items1 = 0;
	items2 = 0;
	table1.resize(size); 
	table2.resize(size);
}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	//load factor
	float load1 = items1 / ((float) table1.size());
	float load2 = items2 / ((float) table2.size());
	//index calculations
	size_t index1 = hash1(key) % table1.size();
	size_t index2 = hash2(key) % table2.size();
	//collision counter
	size_t collision1 = 0;	
	size_t collision2 = 0; 

	// resize table if load factor is 20% its current size
	if(load1 > (0.20) || load2 > (0.20)){
		this -> resize(table1.size());	
	}
	
	//calculate index
	do{
		//Table1
		//no key found, set new key
		if (table1[index1].key == t1()) {
			table1[index1].key = key;
			items1++;
			return table1[index1].value;
		}
		//if key is found, return key value
		else if(table1[index1].key == key){
			return table1[index1].value;
		}
		collision1++;

		//Table2
		//no key found, set new key
		if (table2[index2].key == t1()){
			table2[index2].key = key;
			items2++;
			return table2[index2].value;
		}
		//if key is found, return key value
		else if (table2[index2].key == key){
			return table2[index2].value;
		}
		collision2++;

		//in case of collision, calculate new index and reloop
		if(collision2 % 2 == 0){
			index1 = (index1 + collision1 * hash2(key)) % table1.size();
			index2 = (index2 + collision2 * hash1(key)) % table2.size(); 
		}
		else{
			index1 = (index1 + collision1 * hash1(key)) % table1.size ();
			index2 = (index2 + collision2 * hash2(key)) % table2.size ();
		}	
	}while(true);
}

template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{
	std::vector<keyVal> tempTable1, tempTable2;
	size_t hash;

	//copy prexisiting value from both tables
	for(auto it = table1.begin(); it != table1.end(); ++it){
		tempTable1.push_back(*it);	
	}
	
	for(auto it = table2.begin(); it != table2.end(); ++it){
		tempTable2.push_back(*it);	
	}

	// Clear main tables
	table1.clear();
	table2.clear();

	//increase main table size
	table1.resize(amount * 2);
	table2.resize(amount * 2);

	// Rehash Table 1 
	for(auto it = tempTable1.begin(); it != tempTable1.end(); ++it){
		hash = hash1(it -> key);
		table1[hash % table1.size()] = *it;
	}
	// Rehash Table 2
	for(auto it = tempTable2.begin(); it != tempTable2.end(); ++it){
		hash = hash2(it -> key);
		table1[hash % table1.size()] = *it;
	}

}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash1(std::string key)
{
	// Hash Function 1
	size_t k = key.length(), sum = 0;
	for(size_t i = 0; i <= key.length(); i++){
		sum += (key[i] * pow(10, i));	
	}
	return sum;
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash2(std::string key)
{
	// Hash Function 2
	size_t k = key.length(); 
	size_t sum = 0;
	for(size_t i = 0; i <= key.length(); i++){
		sum += (key[k - 1 - i] * pow(10, i));	
	} 
	return sum;
}