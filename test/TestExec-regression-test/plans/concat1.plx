<?xml version="1.0" encoding="UTF-8"?><!-- Generated by PlexiLisp --><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator"><Node NodeType="Assignment"><NodeId>concat1</NodeId><VariableDeclarations>
      <DeclareVariable>
        <Name>foo</Name>
        <Type>String</Type>
      </DeclareVariable>
      <DeclareVariable>
        <Name>bar</Name>
        <Type>String</Type>
        <InitialValue>
<StringValue>world</StringValue>
        </InitialValue>
      </DeclareVariable>
    </VariableDeclarations><PostCondition><EQString><StringValue>helloworld</StringValue><StringVariable>foo</StringVariable></EQString></PostCondition><NodeBody>
      <Assignment>
        <StringVariable>foo</StringVariable>
        <StringRHS>
          <Concat>
<StringValue>hello</StringValue>
            <StringVariable>bar</StringVariable>
          </Concat>
        </StringRHS>
      </Assignment>
    </NodeBody></Node></PlexilPlan>