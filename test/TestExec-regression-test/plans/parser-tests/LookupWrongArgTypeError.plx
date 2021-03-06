<?xml version="1.0" encoding="UTF-8"?>
<!-- Generated by PlexiLisp -->
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator">
  <GlobalDeclarations>
    <StateDeclaration>
      <Name>foo</Name>
      <Return>
        <Type>Integer</Type>
      </Return>
      <Parameter>
        <Type>Boolean</Type>
      </Parameter>
    </StateDeclaration>
  </GlobalDeclarations>
  <Node NodeType="Assignment">
    <NodeId>LookupWrongArgTypeError</NodeId>
    <VariableDeclarations>
      <DeclareVariable>
        <Name>x</Name>
        <Type>Integer</Type>
      </DeclareVariable>
    </VariableDeclarations>
    <StartCondition>
      <IsKnown>
        <LookupNow>
          <Name>
            <StringValue>foo</StringValue>
          </Name>
          <Arguments>
            <IntegerValue>42</IntegerValue>
          </Arguments>
        </LookupNow>
      </IsKnown>
    </StartCondition>
    <NodeBody>
      <Assignment>
        <IntegerVariable>x</IntegerVariable>
        <NumericRHS>
          <LookupOnChange>
            <Name>
              <StringValue>foo</StringValue>
            </Name>
            <Arguments>
              <IntegerValue>42</IntegerValue>
            </Arguments>
          </LookupOnChange>
        </NumericRHS>
      </Assignment>
    </NodeBody></Node></PlexilPlan>
