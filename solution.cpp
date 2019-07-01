#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */




class CVATRegister
{
  private:
    struct TCompany {
    public:
        string C_NameAddr;
        string C_taxID;
        int invoiceSum;
    };

    vector <TCompany> companiesByName;
    vector <TCompany> companiesByTaxID;

    vector<unsigned int> invoices;


  public:
                  CVATRegister   ( void ){
                    //not neccessary
                  }

                  ~CVATRegister  ( void ){
                    //same as constructor
                  }


    bool          NewCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID ){
      TCompany newCompany;
      vector<TCompany>::iterator whereName, whereTaxID;

      //overit vstupne data
      if (name == "" || addr == "" || taxID == "") {
        return false;
      }


      //vlozenie do struktury
      string nameAddr = name + "=#=" + addr;
      transform(nameAddr.begin(), nameAddr.end(), nameAddr.begin(),::tolower);
      newCompany.C_NameAddr = nameAddr;
      newCompany.C_taxID = taxID;
      newCompany.invoiceSum = 0;

      //overit duplicity
      if ( binary_search( companiesByName.begin(),  companiesByName.end(),  newCompany, cmpName() ) ) {
        return false;
      }
      if ( binary_search( companiesByTaxID.begin(), companiesByTaxID.end(), newCompany, cmpTaxID()) ) {
        return false;
      }

      //najst pozicie kde sa ma vkladat aby boli vektory zoradene (pre oba vektory)
      whereName = lower_bound( companiesByName.begin(), companiesByName.end(), newCompany, cmpName() );
      whereTaxID = lower_bound( companiesByTaxID.begin(), companiesByTaxID.end(), newCompany, cmpTaxID() );

      //vlozenie na najdene pozicie
      companiesByName.insert(whereName,newCompany);
      companiesByTaxID.insert(whereTaxID,newCompany);

      return true;
    }


    bool          CancelCompany  ( const string    & name,
                                   const string    & addr ){
      //overenie vstupu
      if(name == "" || addr == "") {
        return false;
      }

      vector<TCompany>::iterator whereToDelete;
      string nameAddr = name + "=#=" + addr;
      transform(nameAddr.begin(), nameAddr.end(), nameAddr.begin(),::tolower);
      TCompany company;
      company.C_NameAddr = nameAddr;

      //overit ci dana spolocnost existuje
      if ( !binary_search(companiesByName.begin(), companiesByName.end(), company, cmpName()) ) {
        return false;
      }

      //najdenie a vymazanie - z oboch databazi
      whereToDelete = lower_bound( companiesByName.begin(), companiesByName.end(), company, cmpName() );
      company.C_taxID = whereToDelete->C_taxID;
      companiesByName.erase(whereToDelete);

      whereToDelete = lower_bound( companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID() );
      companiesByTaxID.erase(whereToDelete);

      return true;

    }


    bool          CancelCompany  ( const string    & taxID ){
      //overenie vstupu
      if (taxID == "") {
        return false;
      }

      vector<TCompany>::iterator whereToDelete;
      TCompany company;
      company.C_taxID = taxID;

      //overit ci dana spolocnost existuje
      if ( !binary_search(companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID()) ) {
        return false;
      }

      //najdenie a vymazanie
      whereToDelete = lower_bound( companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID() );
      company.C_NameAddr = whereToDelete->C_NameAddr;
      companiesByTaxID.erase(whereToDelete);

      whereToDelete = lower_bound( companiesByName.begin(), companiesByName.end(), company, cmpName() );
      companiesByName.erase(whereToDelete);

      return true;
    }


    bool          Invoice        ( const string    & taxID,
                                   unsigned int      amount ){
      if (taxID == "") {
        return false;
      }

      vector<TCompany>::iterator whereToAdd;
      TCompany company;
      
      company.C_taxID = taxID;

      //vyhladat danu spolocnost v db ci existuje
      if( !binary_search(companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID()) ) return false;

      //pridat fakturu k suctu (aj v druhej db!!!)
      whereToAdd = lower_bound( companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID() );
      whereToAdd->invoiceSum += amount;
        company.C_NameAddr = whereToAdd->C_NameAddr;

      whereToAdd = lower_bound( companiesByName.begin(), companiesByName.end(), company, cmpName() );
      whereToAdd->invoiceSum += amount;


      //pridat fakturu do vektora faktur
      invoices.push_back(amount);

      return true;
    }


    bool          Invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount ){
      if(name == "" || addr == "") return false;

      //premenna
      vector<TCompany>::iterator whereToAdd;
      string nameAddr = name + "=#=" + addr;
      transform(nameAddr.begin(), nameAddr.end(), nameAddr.begin(),::tolower);
      TCompany company;
      company.C_NameAddr = nameAddr;

      //vyhladat danu spolocnost v db ci existuje
      if ( !binary_search(companiesByName.begin(), companiesByName.end(), company, cmpName()) ) {
        return false;
      }

      //pridat fakturu k suctu (aj v druhej db!!!)
      whereToAdd = lower_bound( companiesByName.begin(), companiesByName.end(), company, cmpName() );
      whereToAdd->invoiceSum += amount;

      company.C_taxID = whereToAdd->C_taxID;

      whereToAdd = lower_bound( companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID() );
      whereToAdd->invoiceSum += amount;

      //pridat fakturu do vektora faktur
      invoices.push_back(amount);

      return true;
    }


    bool          Audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const  {
        if(name == "" || addr == "") return false;

        //premenna
        vector<TCompany>::iterator whereToRead;
        string nameAddr = name + "=#=" + addr;
        transform(nameAddr.begin(), nameAddr.end(), nameAddr.begin(),::tolower);
        TCompany company;
        company.C_NameAddr = nameAddr;

        //vyhladat danu spolocnost v db ci existuje
        if( !binary_search(companiesByName.begin(), companiesByName.end(), company, cmpName()) ) return false;

        //precitanie udajov z db
        //whereToRead = lower_bound( companiesByName.begin(), companiesByName.end(), company, cmpName());
        //sumIncome = whereToRead->invoiceSum;
        sumIncome = lower_bound( companiesByName.begin(), companiesByName.end(), company, cmpName())->invoiceSum;

        return true;
    }


    bool          Audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const  {
        if (taxID == "") {
          return false;
        }

        vector<TCompany>::iterator whereToRead;
        TCompany company;
        company.C_taxID = taxID;

        //vyhladat danu spolocnost v db ci existuje
        if ( !binary_search(companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID()) ) {
          return false;
        }

        //precitanie udajov z db
        sumIncome = lower_bound( companiesByTaxID.begin(), companiesByTaxID.end(), company, cmpTaxID() )->invoiceSum;

        return true;
    }


    unsigned int  MedianInvoice  ( void ) const {
      unsigned int median = 0;

       if (invoices.size() == 0) {
        return 0;
      }

      //skopirovanie vektora s fakturami
      vector<unsigned int> sortedInvoices = invoices;

      //sortenie skopirovaneho vektora
      sort(sortedInvoices.begin(),sortedInvoices.end());

      median = sortedInvoices[((int) sortedInvoices.size()/2)];
      
      return median;
    }

    //custom
    void printDB(void){
      cout << "= firmy podla mena = " << endl;
      for (unsigned int i = 0; i < companiesByName.size(); i++)
        cout << companiesByName[i].C_NameAddr << "\t" << companiesByName[i].C_taxID << "\t(" << companiesByName[i].invoiceSum << ")" << endl;

      cout << "= firmy podla taxID = " << endl;
      for (unsigned int i = 0; i < companiesByTaxID.size(); i++)
        cout << companiesByTaxID[i].C_NameAddr << "\t" << companiesByTaxID[i].C_taxID << "\t(" << companiesByTaxID[i].invoiceSum << ")" << endl;

      cout << "= fakturky =" << endl;
      for(unsigned int i = 0; i < invoices.size(); ++i)
        cout << invoices[i] << endl;
    }

    //porovnavacie funkcie
    struct cmpName
    {
        bool operator() (const TCompany & comp, const string & str) const {
          return comp.C_NameAddr < str;
        }
        bool operator() (const string & str, const TCompany & comp) const {
          return comp.C_NameAddr < str;
        }
        bool operator() (const TCompany & comp1, const TCompany & comp2) const {
          return comp1.C_NameAddr < comp2.C_NameAddr;
        }
    };
    struct cmpTaxID
    {
        bool operator() (const TCompany & comp, const string & str) const {
          return comp.C_taxID < str;
        }
        bool operator() (const string & str, const TCompany & comp) const {
          return comp.C_taxID < str;
        }
        bool operator() (const TCompany & comp1, const TCompany & comp2) const {
          return comp1.C_taxID < comp2.C_taxID;
        }
    };

};




#ifndef __PROGTEST__
int main ( void )
{
  unsigned int sumIncome;


    CVATRegister b1;
    assert ( b1 . NewCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . NewCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . NewCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . Invoice ( "666/666", 2000 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "666/666/666", 3000 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "123456", 4000 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . MedianInvoice () == 4000 );
    assert ( b1 . Audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . Audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . CancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . MedianInvoice () == 4000 );
    assert ( b1 . CancelCompany ( "666/666" ) );
    assert ( b1 . MedianInvoice () == 4000 );
    assert ( b1 . Invoice ( "123456", 100 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "123456", 300 ) );
    assert ( b1 . MedianInvoice () == 3000 );
    assert ( b1 . Invoice ( "123456", 200 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "123456", 230 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "123456", 830 ) );
    assert ( b1 . MedianInvoice () == 830 );
    assert ( b1 . Invoice ( "123456", 1830 ) );
    assert ( b1 . MedianInvoice () == 1830 );
    assert ( b1 . Invoice ( "123456", 2830 ) );
    assert ( b1 . MedianInvoice () == 1830 );
    assert ( b1 . Invoice ( "123456", 2830 ) );
    assert ( b1 . MedianInvoice () == 2000 );
    assert ( b1 . Invoice ( "123456", 3200 ) );
    assert ( b1 . MedianInvoice () == 2000 );


  CVATRegister b2;
  assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . NewCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . NewCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . NewCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . MedianInvoice () == 0 );
  assert ( b2 . Invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . MedianInvoice () == 1000 );
  assert ( b2 . Invoice ( "abcdef", 2000 ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( b2 . Invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( ! b2 . Invoice ( "1234567", 100 ) );
  assert ( ! b2 . Invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . Invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . Audit ( "1234567", sumIncome ) );
  assert ( ! b2 . Audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . Audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . CancelCompany ( "1234567" ) );
  assert ( ! b2 . CancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . CancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . CancelCompany ( "abcdef" ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( ! b2 . CancelCompany ( "abcdef" ) );
  assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . CancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . CancelCompany ( "ACME", "Kolejni" ) );

  return 0;
}
#endif /* __PROGTEST__ */
