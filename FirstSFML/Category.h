#ifndef CATEGORY_H
#define CATEGORY_H


/*
 * Entity/scene node category, used to dispatch commands to the intended targets.
 * Provides a simple interface that utilizes bit flags to determine which targets recieve
 * which commands.
 */
 namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0, //think bits! start with 1 (ie 0001) and left shift by 0 bits (remains 1 = 0001)
		PlayerAircraft = 1 << 1, //start with 1 (ie 0001) and leftshift by 1 bit (becomes 2 = 0010)
		AlliedAircraft = 1 << 2, //start with 1 (ie 0001) and leftshift by 2 bits (becomes 4 = 0100)
		EnemyAircraft = 1 << 3,  //start with 1 (ie 0001) and leftshift by 3 bits (becomes 8 = 1000)
		Pickup = 1 << 4,
		AlliedProjectile = 1 << 5,
		EnemyProjectile = 1 << 6,

		Aircraft = PlayerAircraft | AlliedAircraft | EnemyAircraft,
		Projectile = AlliedProjectile | EnemyProjectile,
	};

}

#endif // CATEGORY_H

	//EXPLANATION OF THE LOGIC:

	//the above syntax allows us to combine different categories together:
	//ie, to send command to all available planes:
	//unsigned int anyAircraft = Category::PlayerAircraft 
							//	| Category::AlliedAircraft
							//  | Category::EnemyAircraft

	//unsigned int playerSquadron = Category::PlayerAircraft
								//  | Category::AlliadAircraft

	//so therefor, playerSquadron is itself an unsigned int but we're using it as a bit holder:
	//PlayerAircraft = 2 (ie, 0010), and AlliedAircraft = 4 (ie, 0100) (while EnemyAircraft = 8 (ie, 1000)
	//Therefore, by setting playerSquadron as PlayerAir | AlliedAir, we're adding the bits of both
	//so "playerSquadron" becomes 6 (ie, 0110)
	//this is useful, because now "playerSquadron" can be used as a map of sorts
	//Our Command structure will internally deal with this when we set command.category = playerSquadron,
	//but basically, from "playerSquadron" (ie, 0110), we know to to apply the command
	//to any Category::Type that has a 1 bit in EITHER place. 
	//ie, any Category::Type == 0100 (AlliedAircraft)
	//or      Category::Type == 0010 (PlayerAircraft)

	//the downfall of this, is we must remain careful!
	//we can only hold as many types as the longest unsigned int we can use is 
	//ie, however many bits there are in the unsigned int(32, or 64. etc)
	//If we have any value that has more than one bit set to 1, we will get
	//mega logic errors
	 
