/* Copyright (c) 2006-2015, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package reader;

import java.util.Vector;

import net.n3.nanoxml.*;

import model.expr.Expr;
import model.expr.ExprType;
import model.GlobalDecl;
import model.GlobalDecl.CallType;
import model.GlobalDeclList;
import model.Var;
import model.Var.VarMod;
import model.VarList;

public class DeclReader {
	public static final String NameDeclarationText = "Name";
	public static final String TypeDeclarationText = "Type";
	public static final String ParameterDeclarationText = "Parameter";
	public static final String ReturnDeclarationText = "Return";
	public static final String InterfaceDeclarationText = "Interface";
	public static final String InDeclarationText = "In";
	public static final String InOutDeclarationText = "InOut";

	public GlobalDeclList xmlToDecls(IXMLElement declXml) {
		GlobalDeclList decls = new GlobalDeclList();
		if (declXml == null)
			return decls;
		
		for (IXMLElement child : getChildren(declXml)) {
			GlobalDecl d = convertXmlToDec(child);
			if (d != null)
				decls.add(d);
		}
		return decls;
	}

	private GlobalDecl convertXmlToDec(IXMLElement xml) {
        String tag = xml.getName();
		if (xml.isLeaf()) {
            System.out.println("Reader error: Empty " + tag + " declaration");
			return null;
        }

        switch (tag) {
		case "CommandDeclaration":
            return parseCommandDec(xml);

        case "StateDeclaration":
            return parseStateDec(xml);
            
        case "LibraryNodeDeclaration":
            return parseLibraryDec(xml);

        default:
            System.out.println("Reader error: unknown declaration type " + tag);
            return null;
        }
	}

	private GlobalDecl parseCommandDec(IXMLElement xml) {
		String id = null;
		VarList params = new VarList();
		VarList rets = new VarList();
		for (IXMLElement child : getChildren(xml)) {
			String text = child.getName();
            switch (text) {
            case "Parameter":
                params.add(convertXmlToParam(child));
                break;
                
            case "Return":
                rets.add(convertXmlToParam(child));
                break;

            case "Name":
                id = child.getContent();
                break;

            case "ResourceList":
                // TODO
                break;

            default:
                System.out.println("Reader error: illegal element " + text
                                   + " in " + xml.getName());
                break;
            }
		}

        // check that we have a name
        if (id == null) {
            System.out.println("DeclReader.convertXmlToLibraryDec: required Name element missing in "
                               + xml.getName() + " declaration");
            return null;
        }

		return new GlobalDecl(CallType.Command, id, params, rets);
    }

	private GlobalDecl parseStateDec(IXMLElement xml) {
		String id = null;
		VarList params = new VarList();
		VarList rets = new VarList();
		for (IXMLElement child : getChildren(xml)) {
			String text = child.getName();
            switch (text) {
            case "Parameter":
                params.add(convertXmlToParam(child));
                break;
                
            case "Return":
                rets.add(convertXmlToParam(child));
                break;

            case "Name":
                id = child.getContent();
                break;

            default:
                System.out.println("Reader error: illegal element " + text
                                   + " in " + xml.getName());
                break;
            }
		}

        // check that we have a name
        if (id == null) {
            System.out.println("DeclReader.convertXmlToLibraryDec: required Name element missing in "
                               + xml.getName() + " declaration");
            return null;
        }

		return new GlobalDecl(CallType.Lookup, id, params, rets);
    }

	private GlobalDecl parseLibraryDec(IXMLElement xml) {
		String id = null;
		VarList params = new VarList();
		for (IXMLElement child : getChildren(xml)) {
			String text = child.getName();
            switch (text) {
            case "Name":
                id = child.getContent();
                break;
                
            case "Interface":
                params.addAll(convertXmlToInterfaceVars(child));
                break;

            default:
                System.out.println("Reader error: illegal element " + text
                                   + " in " + xml.getName());
                break;
            }
		}

        // check that we have a name
        if (id == null) {
            System.out.println("Reader error: required Name element missing in "
                               + xml.getName());
            return null;
        }

        return new GlobalDecl(CallType.LibraryCall, id, params, null);
    }
	
	private Var convertXmlToParam(IXMLElement xml) {
		if (xml == null)
			return null;

		IXMLElement typeElt = xml.getFirstChildNamed(TypeDeclarationText);
		if (typeElt == null) {
            System.out.println("Reader error: required Type element missing in parameter declaration");
            return null;
		}

		ExprType type = ExprType.typeForName(typeElt.getContent());
        if (type == null) {
            System.out.println("Reader error: invalid type name " + typeElt.getContent()
                               + " in parameter declaration");
            return null;
        }

		IXMLElement nameElt = xml.getFirstChildNamed(NameDeclarationText);
		if (nameElt == null)
            return new Var(type);
        return new Var(nameElt.getContent(), type);
	}
	
	public IXMLElement findXmlByTerm(IXMLElement xml, String term)
	{
		if (xml == null)
			return null;
		
		if (xml.getName().equals(term))
			return xml;
		
		for (IXMLElement child : getChildren(xml))
		{
			IXMLElement result = findXmlByTerm(child, term);
			if (result != null)
				return result;
		}
		return null;
	}
	
	private VarList convertXmlToInterfaceVars(IXMLElement xml)
	{
		if (xml == null)
			return null;
		
		VarList list = new VarList();
		for (IXMLElement varXml : getChildren(xml)) {
			if (varXml.getName() == null)
				continue;
			
			VarMod mod = VarMod.None;
			if (varXml.getName().equals(InDeclarationText))
				mod = VarMod.In;
			else if (varXml.getName().equals(InOutDeclarationText))
				mod = VarMod.InOut;
			
			// If there is no declaration, there's no Var to create
			if (!varXml.hasChildren() || !varXml.getChildAtIndex(0).hasChildren())
				continue;
			
			String name = null;
			ExprType type = null;
			// Skip over an XML level that holds DeclareVariable, hence .getChildAtIndex(0)
			for (IXMLElement child : getChildren(varXml.getChildAtIndex(0))) {
                if (child.getName().equals(NameDeclarationText))
                    name = child.getContent();
                if (child.getName().equals(TypeDeclarationText)) {
                    String typeStr = child.getContent();
                    if (typeStr != null)
                        type = ExprType.typeForName(typeStr);
                }
            }
			
			if (name != null)
				list.add(new Var(name, type, mod));
			else 
                System.out.println("Reader error: unnamed " + type.toString()
                                   + " parameter in " + varXml.getName() + " Interface declaration");
		}
		return list;
	}
	
	// IXMLElement.getChildren() only returns Vector. Having a typed vector is nicer, so we type cast here.
	@SuppressWarnings("unchecked")
	private Vector<IXMLElement> getChildren(IXMLElement elem)
	{
		Vector<IXMLElement> children = elem.getChildren();
		return children;
	}

}
