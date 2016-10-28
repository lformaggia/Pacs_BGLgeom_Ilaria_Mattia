#ifndef HH_MUPARSERINTERFACE_HH
#define HH_MUPARSERINTERFACE_HH
#include <string>
#include "muParser.h"

namespace MuParserInterface
{
  template<unsigned int dim>
  class muParserInterface
  {
  public:
    muParserInterface(); //! constructor
    muParserInterface(const std::string & s); //! constructor with string
    ~muParserInterface(); //! destructor
    muParserInterface(muParserInterface const &); //! copy constructor
    muParserInterface & operator=(muParserInterface const &);  //! operator =
    
    void set_expression(const std::string & e);
    
    double operator()(double const t);

  private:
    mu::Parser M_parser;
    double M_t;
    std::string M_expr;  //! string containing the expressions of the dim components separated by commas
  }; // class


//! Definition of the methods

  //! Default constructor
  muParserInterface::muParserInterface()
  {
    this->M_parser.DefineVar("t",&M_t);
  }


  //! constuctor given a string
  muParserInterface::muParserInterface(const std::string & e):
    muParserInterface()
  { 
    M_expr=e;
    this->M_parser.SetExpr(e);
  }


  //! destructor
  muParserInterface::~muParserInterface()
  {
    this->M_parser.ClearVar();
  }


  //! Copy constructor
  muParserInterface::muParserInterface(muParserInterface const & mpi):
    M_parser(),M_t(mpi.M_t),M_expr(mpi.M_expr)
  {
    this->M_parser.SetExpr(M_expr);
    this->M_parser.DefineVar("t",&M_t);   
  }


  //! operator =
  muParserInterface & muParserInterface::operator=(muParserInterface const & mpi)
  {
    if (this != &mpi)
      {
	this->M_parser.ClearVar();
	this->M_expr=mpi.M_expr;
	this->M_t=mpi.M_t;
	this->M_parser.SetExpr(M_expr);
	this->M_parser.DefineVar("t",&M_t);
      }
    return *this;
  }


  //! Set expresion
  void  muParserInterface::set_expression(const std::string & s)
  {
    M_expr=s;
    this->M_parser.SetExpr(s);
  }


  //! operator ()
  double muParserInterface::operator()(double const t)
  {
    this->M_t=t;
    
    /* OLNY FOR DEBUGGING
    // Get the map with the variables
    mu::varmap_type variables = M_parser.GetVar();
    std::cout << "Number: " << (int)variables.size() << "\n";
    
    // Get the number of variables 
    mu::varmap_type::const_iterator item = variables.begin();
    
    // Query the variables
    for (; item!=variables.end(); ++item)
      {
	std::cout << "Name: " << item->first << " Value" << *item->second << "\n";
      }
    */
    
    std::array<double,dim> point_coords(this->M_parser.Eval(dim));
    BGLgeom::point<dim> P(point_coords);
    return P;
  }

  //! prints message on the standard error
  void printMuException(mu::Parser::exception_type &e)
  {
    using std::cout;
    cout << "Content of muParser exception" << "\n";
    cout << "Message:  " << e.GetMsg() << "\n";
    cout << "Formula:  " << e.GetExpr() << "\n";
    cout << "Token:    " << e.GetToken() << "\n";
    cout << "Position: " << e.GetPos() << "\n";
    cout << "Errc:     " << e.GetCode() << "\n";
  }
}// end namespace
#endif
