/*----------------------------------------------------------------------------

Name		parser.cpp

Purpose		Returns text related to the given search criteria.  Text must
            follow the form:

            port cob-id [#] XX XX XX XX XX XX XX XX

            or

            (XXXXXX) port cob-id [#] XX XX XX XX XX XX XX XX

            Where X = don't care

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/

#include "parser.h"

/*----------------------------------------------------------------------------

Name		Parser

Purpose		Constructor

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
Parser::Parser( )
    : mMap( createRefMap() )
{

}

/*----------------------------------------------------------------------------

Name		setText

Purpose		Set the text for the parser to parse

Input       text - Text to parse

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::setText(const QString& text)
{
    mText = text;
    parse();
}

/*----------------------------------------------------------------------------

Name		splitOnNonAlphaNum

Purpose		Split a string on non-alphanumeric characters

Input       str - str to split

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
QStringList splitOnNonAlphaNum( QString str )
{
    return str.split(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")), QString::SkipEmptyParts);
}

/*----------------------------------------------------------------------------

Name		splitOnWhiteSpace

Purpose		Split a string on white space

Input       str - str to split

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
QStringList splitOnWhiteSpace( QString str )
{
    return str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
}

/*----------------------------------------------------------------------------

Name		setPort

Purpose		Set the ports to filter

Input       port - string containing all ports to filter

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::setPort(QString port)
{
    mPorts = splitOnNonAlphaNum(port);
    parse();
}

/*----------------------------------------------------------------------------

Name		setAddr

Purpose		Set the addresses to filter

Input       addr - string containing all addresses to filter

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::setAddr(QString addr)
{
    mAddrs = splitOnNonAlphaNum(addr);
    parse();
}

/*----------------------------------------------------------------------------

Name		setObjIdx

Purpose		Set the object indices of SDOs to filter

Input       objIdx - string containing all object indices to filter

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::setObjIdx(QString objIdx)
{
    mObjIdxs = splitOnNonAlphaNum(objIdx);
    parse();
}

/*----------------------------------------------------------------------------

Name		setSubIdx

Purpose		Set the sub indices to filter

Input       subIdx - string containing all sub indices to filter

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::setSubIdx(QString subIdx)
{
    mSubIdxs = splitOnNonAlphaNum(subIdx);
    parse();
}

/*----------------------------------------------------------------------------

Name		addType

Purpose		Add a type of packet to filter

Input       type - type of packet to add to filter

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::addType(PacketType type)
{
    if ( !mTypes.contains( type ) )
        mTypes.push_back( type );

    parse();
}

/*----------------------------------------------------------------------------

Name		removeType

Purpose		Remove a type of packet from filter

Input       type - type of packet to remove from filter

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::removeType(PacketType type)
{
    if ( mTypes.contains( type ) )
        mTypes.remove( mTypes.indexOf( type )  );

    parse();
}

/*----------------------------------------------------------------------------

Name		createRefMap

Purpose		Generate the const reference map (assigns type values to enum)

Return      Map< PacketType, int > - hard defined map of enums to int

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
PktMap Parser::createRefMap()
{
    PktMap map;
    map.insert( NMT,            0 );
    map.insert( EMER,        0x80 );
    map.insert( TIME,       0x100 );
    map.insert( T_SDO,      0x580 );
    map.insert( R_SDO,      0x600 );
    map.insert( T_PDO_1,    0x180 );
    map.insert( R_PDO_1,    0x200 );
    map.insert( T_PDO_2,    0x280 );
    map.insert( R_PDO_2,    0x300 );
    map.insert( T_PDO_3,    0x380 );
    map.insert( R_PDO_3,    0x400 );
    map.insert( T_PDO_4,    0x480 );
    map.insert( R_PDO_4,    0x500 );
    map.insert( NODE_GUARD, 0x700 );

    return map;
}

/*----------------------------------------------------------------------------

Name		checkPort

Purpose		Returns true if the port found in the given QStringList is within
            the filter

Input       lst - Tokenized line of file

Return      true if port is in test list or the list is empty, false
            if not

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
bool Parser::checkPort( QStringList lst )
{
    if ( mPorts.isEmpty() )
        return true;

    int idx;
    ( mHasTimeStamp ) ? idx = 1 : idx = 0;

    return mPorts.contains( lst.at(idx), Qt::CaseInsensitive );
}

/*----------------------------------------------------------------------------

Name		checkAddr

Purpose		Returns true if the address found in the given QStringList is within
            the filter

Input       lst - Tokenized line of file

Return      true if address is in test list or the list is empty, false
            if not

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
bool Parser::checkAddr(QStringList lst)
{
    if ( mAddrs.isEmpty() )
        return true;

    int idx;
    ( mHasTimeStamp ) ? idx = 2 : idx = 1;

    uint cobid = lst.at(idx).toInt(0, 16);
    uint addr = (cobid & 0x7F);

    QString straddr = QString::number( addr, 16 );

    return mAddrs.contains( straddr, Qt::CaseInsensitive );
}

/*----------------------------------------------------------------------------

Name		checkObjIdx

Purpose		Returns true if the object index found in the given QStringList is
            within the filter

Input       lst - Tokenized line of file

Return      true if object index is in test list or the list is empty, false
            if not

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
bool Parser::checkObjIdx( QStringList lst )
{
    if ( mObjIdxs.isEmpty() )
        return true;

    if ( !isSdo( lst ) )
        return false;

    int idx;
    ( mHasTimeStamp ) ? idx = 5 : idx = 4;

    QString objIdx = lst.at(idx + 1) + lst.at(idx);

    return mObjIdxs.contains( objIdx, Qt::CaseInsensitive );
}

/*----------------------------------------------------------------------------

Name		checkSubIdx

Purpose		Returns true if the subindex found in the given QStringList is
            within the filter

Input       lst - Tokenized line of file

Return      true if subindex is in test list or the list is empty, false
            if not

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
bool Parser::checkSubIdx(QStringList lst)
{
    if ( mSubIdxs.isEmpty() )
        return true;

    if ( !isSdo( lst ) )
        return false;

    int idx;
    ( mHasTimeStamp ) ? idx = 7 : idx = 6;

    return mSubIdxs.contains( lst.at(idx), Qt::CaseInsensitive );
}

/*----------------------------------------------------------------------------

Name		checkType

Purpose		Returns true if the type found in the given QStringList is within
            the filter

Input       lst - Tokenized line of file

Return      true if type is in test list or the list is empty, false
            if not

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
bool Parser::checkType( QStringList lst )
{
    if ( mTypes.isEmpty() )
        return true;

    int idx;
    ( mHasTimeStamp ) ? idx = 2 : idx = 1;

    uint cobid = lst.at(idx).toInt(0, 16);
    uint func = cobid & 0xF80;

    for ( int i = 0; i < mTypes.size(); ++i )
    {
        if ( func == mMap.value( mTypes.at(i) ) )
             return true;
    }

    return false;
}

/*----------------------------------------------------------------------------

Name		isSdo

Purpose		Returns true if the line is a type of SDO message

Input       lst - Tokenized line of file

Return      true if type is in test list or the list is empty, false
            if not

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
bool Parser::isSdo(QStringList lst)
{
    int idx;
    ( mHasTimeStamp ) ? idx = 2 : idx = 1;

    uint cobid = lst.at(idx).toInt(0, 16);
    uint func = cobid & 0xF80;

    return ( func == 0x600 || func == 0x580 );
}

/*----------------------------------------------------------------------------

Name		parse

Purpose		Parses the text found in mText.  Emits a signal containing parsed
            text when finished.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void Parser::parse()
{
    if ( !mText.isEmpty() )
    {
        mParsed.clear();

        QStringList txt = mText.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

        if ( txt.at(0).contains("(") )
        {
            mHasTimeStamp = true;
        }

        for ( int i = 0; i < txt.size(); ++i )
        {
            bool valid = true;

            QStringList line = splitOnWhiteSpace(txt.at(i).trimmed());

            valid &= checkPort( line );
            valid &= checkAddr( line );

            valid &= checkObjIdx( line );
            valid &= checkSubIdx( line );

            valid &= checkType( line );

            if ( valid )
                mParsed += txt.at(i) + "\r";
        }

        emit textChanged( mParsed );
    }
}
