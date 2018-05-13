/*---------------------------------------------- -----------------------------

Name		parser.h

Purpose		Returns text related to the given search criteria.  Text must
            follow the form:

            port cob-id [#] XX XX XX XX XX XX XX XX

            or

            (XXXXXX) port cob-id [#] XX XX XX XX XX XX XX XX

            Where X = don't care

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
#ifndef PARSER_H
#define PARSER_H

#include <QMap>
#include <QObject>
#include <QPair>
#include <QStringList>
#include <QVector>


// Specifies variety of available, standard, packet types
enum PacketType
    {

    NMT,        // NMT messages
    EMER,       // Emergency messages
    TIME,       // Timing messages

    R_SDO,      // Receive SDO messages
    R_PDO_1,    // Receive PDO messages
    R_PDO_2,
    R_PDO_3,
    R_PDO_4,

    T_SDO,      // Transmit SDO messages
    T_PDO_1,    // Transmit PDO messages
    T_PDO_2,
    T_PDO_3,
    T_PDO_4,

    NODE_GUARD  // Node guard messages
    };

// Convenience typedef
typedef QMap< PacketType, int > PktMap;

class Parser : public QObject
{
    Q_OBJECT

public:
    explicit Parser();
    ~Parser(){}

    // Set the text of the parser (base text that will be parsed)
    void setText( const QString& text );

    // Set the ports that will make it through the filter
    void setPort( QString port );
    // Set the addresses that will make it through the filter
    void setAddr( QString addr );

    // Sets the object indices that will make it through the filter
    void setObjIdx( QString objIdx );
    // Sets the subindices that will make it through the filter
    void setSubIdx( QString subIdx );

    // Add a type to the filter (these will be allowed through)
    void addType( PacketType type );
    // Remove a type from the types
    void removeType( PacketType type );

signals:
    // emitted whenever the parsing is complete
    void textChanged( const QString& text );

private:
    // creates a map to reference when parsing types
    PktMap createRefMap();

    // Checks the inputted line for a port match
    bool checkPort( QStringList lst );
    // Checks the inputted line for a address match
    bool checkAddr( QStringList lst );
    // Checks the inputted line for a object index match
    bool checkObjIdx( QStringList lst );
    // Checks the inputted line for a subindex match
    bool checkSubIdx( QStringList lst );
    // Checks the inputted line for a type match
    bool checkType( QStringList lst );

    // Checks whether the inputted line is an SDO
    bool isSdo( QStringList lst );

    // Parses a given string
    void parse();

private:
    QString mText;                  // Base text to parse
    QString mParsed;                // Parsed text

    QStringList mPorts;             // Ports to filter against
    QStringList mAddrs;             // Addresses to filter against

    QStringList mObjIdxs;           // Object indices to filter against
    QStringList mSubIdxs;           // Subindices to filter against

    QVector<PacketType> mTypes;     // Types to filter against
    const PktMap mMap;              // Map to reference for ints corresponding to packet types

    bool mHasTimeStamp;             // True if the parsed text contains a timestamp

};

#endif // PARSER_H
