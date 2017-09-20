
import java.io.*;

/** This is an example of object serialization that will make saving
 *  your model very easy.
 *
 *  Run this test program with:
 *    java -classpath . Model
 *
 *  See the following classes in the java.io package:
 *    Serializable
 *    ObjectOutputStream
 *    ObjectInputStream
 */

public class Model implements Serializable {
	Character myFavoriteCharacter;
	int numPlayers;
	
	public Model(){
		myFavoriteCharacter = new Character("Travis");
		numPlayers = 1;
	}
	
	public void save(String fileName){
		try {
			FileOutputStream ostream = new FileOutputStream(fileName);
			ObjectOutputStream p = new ObjectOutputStream(ostream);
			
			p.writeObject(this);
	
			p.flush();
			ostream.close();
		}
		catch (IOException ex){
			System.err.println("Could not save file: "+fileName);
			ex.printStackTrace();
		}			
	}
	
	public static Model read(String fileName){
		try {
			FileInputStream istream = new FileInputStream(fileName);
			ObjectInputStream p = new ObjectInputStream(istream);
			
			Model savedModel = (Model)p.readObject();
			
			istream.close();
			return savedModel;
		}
		catch (IOException ex){
			System.err.println("Could not read file: "+fileName);
			ex.printStackTrace();
		}	
		catch (ClassNotFoundException ex){}		
		
		return null;
	}
	
	public static void main(String[] args){
		String fileName = "E:/temp/Model1.buzz";
		Model myModel = new Model();
		myModel.save(fileName);
		
		Model readFromFileModel = Model.read(fileName);
		System.out.println("MyFavoriteCharacter: "+readFromFileModel.myFavoriteCharacter);
		System.out.println("NumPlayers = "+readFromFileModel.numPlayers);
	}
}

class Character implements Serializable {
	String name, imagePath;
	int initialStrength;
	
	public Character(String name){
		this.name = name;
		imagePath = "/temp/"+name;
		initialStrength = 10;
	}
	
	public String toString(){
		return "Name="+name+", imagePath="+imagePath+", initialStrength="+initialStrength;
	}
}