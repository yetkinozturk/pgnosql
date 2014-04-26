class PGConnectionError: public std::exception
{
public:
	explicit PGConnectionError(){}

    explicit PGConnectionError(const char* message):
      msg_(message)
      {
      }

    explicit PGConnectionError(const std::string& message):
      msg_(message)
      {}

    virtual ~PGConnectionError() throw (){}

    virtual const char* what() const throw (){
       return msg_.c_str();
    }

protected:
    std::string msg_;
};

class CommandParseError : public std::exception
{

};

class UnknownCommandError : public std::exception
{

};

class CommandTranslateError : public std::exception
{

};

