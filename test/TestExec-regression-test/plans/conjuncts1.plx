<?xml version="1.0" encoding="UTF-8"?><!-- Generated by PlexiLisp --><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator"><Node NodeType="NodeList"><NodeId>conjuncts1</NodeId><EndCondition><EQInternal><NodeStateVariable><NodeId>ConjunctTest1</NodeId></NodeStateVariable><NodeStateValue>FINISHED</NodeStateValue></EQInternal></EndCondition><NodeBody>
      <NodeList>
        <Node NodeType="Command"><NodeId>ConjunctTest1</NodeId><StartCondition><OR><EQNumeric><LookupNow>
                  <Name>
<StringValue>B</StringValue>
                  </Name>
                </LookupNow><IntegerValue>2</IntegerValue></EQNumeric><EQNumeric><LookupNow>
                  <Name>
<StringValue>B</StringValue>
                  </Name>
                </LookupNow><IntegerValue>2</IntegerValue></EQNumeric><EQNumeric><LookupNow>
                  <Name>
<StringValue>B</StringValue>
                  </Name>
                </LookupNow><IntegerValue>2</IntegerValue></EQNumeric></OR></StartCondition><NodeBody>
            <Command>
              <Name>
<StringValue>Execute1</StringValue>
              </Name>
            </Command>
          </NodeBody></Node>
      </NodeList>
    </NodeBody></Node></PlexilPlan>