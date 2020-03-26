(* mlstone.sml

   MLStone Card format:
   ( cost, spellp, name, attack, health )
     int,  T/F,    string, int,  int
           T for spells          only for spells if heals -- I don't think I will make any of these type though

Game Play Loop:
-----------------------------------------------------------------------------
   set turn to 1
   repeat forever
      set player energy to equal turn number
      print game state
      while player is not finished do  ; PLAYER TURN (read-eval-print loop)
          read player command
          evaluate player command
          if Player is dead, print message and exit
          if AI is dead, print message and exit
          print game state
      set AI energy to equal turn number
      while AI is not finished to	    ; AI TURN
          select AI command
          evaluate AI command
          if Player is dead, print message and exit
          if AI is dead, print message and exit
          print game state
      increment turn
-----------------------------------------------------------------------------
*)

val seed = Random.rand(0,Real.floor(Time.toReal(Time.now())));
val gameOver = false; (* When gameOver = true, then game is over *)
val turnOver = false;
val turn = 0;
val energy = 0;

(* Player cards *)
val card1 = (5, false, "Raging Dragon", 5, 6);
val card2 = (1, false, "Potato", 1, 3);
val card3 = (7, false, "YEET", 10, 9);
val card4 = (4, true, "Fireball", 5, 0);
val card5 = (4, false, "Big Chungus", 1, 10);
val card6 = (1, true, "Wisp", 2, 0);
val card7 = (2, false, "Feral Wolf", 4, 2);
val card8 = (3, false, "Grunt", 2, 5);
val card9 = (3, true, "Ice Blast", 4, 0);
val card10 = (5, false, "Overgrowth", 4, 7);
val card11 = (6, false, "Caster", 7, 6);
val card12 = (6, true, "Blast", 7, 0);
val card13 = (7, false, "Adventurer", 6, 9);
val card14 = (7, true, "Blizzard", 8, 0);
val card15 = (1, false, "Stump", 1, 4);
val card16 = (2, true, "Shout", 3, 0);
val card17 = (2, false, "Fighter", 2, 4);
val card18 = (3, false, "Rider", 4, 3);
val card19 = (4, false, "Golem", 3, 6);
val card20 = (2, false, "Recruit", 3, 2);
val card21 = (4, false, "Archer", 5, 4);
val card22 = (4, true, "Missiles", 5, 0);
val card23 = (3, true, "Bellow", 4, 0);
val card24 = (8, false, "Sentinal Guardian", 9, 9);
val card25 = (9, false, "Assassin", 12, 7);
val card26 = (10, false, "Champion", 15, 15);
val card27 = (1, false, "Toy", 2, 1);
val card28 = (2, true, "Flare", 3, 0);
val card29 = (8, true, "Explosion", 10, 0);
val card30 = (5, false, "Knight", 5, 7);

(* Enemy cards *)
val card31 = (5, false, "Feral Fiend", 5, 6);
val card32 = (1, false, "Imp", 1, 3);
val card33 = (7, false, "Dark Knight", 10, 9);
val card34 = (4, true, "Fireblast", 5, 0);
val card35 = (4, false, "Living Wall", 1, 10);
val card36 = (1, true, "Dark Blast", 2, 0);
val card37 = (2, false, "Crazy Bomber", 4, 2);
val card38 = (3, false, "Goblin", 2, 5);
val card39 = (3, true, "Ice Shards", 4, 0);
val card40 = (5, false, "Twisted Ent", 4, 7);
val card41 = (6, false, "Beholder", 7, 6);
val card42 = (6, true, "Lightning", 7, 0);
val card43 = (7, false, "Giant Snake", 6, 9);
val card44 = (7, true, "Flame Spire", 8, 0);
val card45 = (1, false, "Slime", 1, 4);
val card46 = (2, true, "Hex", 3, 0);
val card47 = (2, false, "Goblin", 2, 4);
val card48 = (3, false, "Goblin Rider", 4, 3);
val card49 = (4, false, "Banshee", 3, 6);
val card50 = (2, false, "Giant Rat", 3, 2);
val card51 = (4, false, "Orc", 5, 4);
val card52 = (4, true, "Hellfire", 5, 0);
val card53 = (3, true, "Curse", 4, 0);
val card54 = (8, false, "Necromancer", 9, 9);
val card55 = (9, false, "Void Abomination", 12, 7);
val card56 = (10, false, "Black Dragon", 15, 15);
val card57 = (1, false, "Scarecrow", 2, 1);
val card58 = (2, true, "Shift", 3, 0);
val card59 = (8, true, "Drain", 10, 0);
val card60 = (5, false, "Giant Spider", 5, 7);

val playerHand = [];
val playerMinionList = [];
val PlayerDeck = [card1, card2, card3, card4, card5, card6, card7, card8, card9, card10, card11, card12, card13, card14, card15, card16, card17, card18, card19, card20, card21, card22, card23, card24, card25, card26, card27, card28, card29, card30];
val playerDeck = ref PlayerDeck;
val enemyHand = [];
val enemyMinionList = [];
val EnemyDeck = [card31, card32, card33, card34, card35, card36, card37, card38, card39, card40, card41, card42, card43, card44, card45, card46, card47, card48, card49, card50, card51, card52, card53, card54, card55, card56, card57, card58, card59, card60];
val enemyDeck = ref EnemyDeck;

(* Basic functions to give player info every turn and to update changing variables *)
fun setTurn(TURN) = TURN + 1;
fun setEnergy(ENERGY) = ENERGY;
fun isPlayerTurn(theTurn) = if theTurn mod 2 = 1 then true else false;    (* Returns true if it is the player's turn, false otherwise *)
fun isEnemyTurn(theTurn) = if theTurn mod 2 = 0 then true else false;
fun printStuff(theTurn) = print ("player turn? --> " ^ Bool.toString(isPlayerTurn(theTurn)) ^ "\n");
fun printTurn(theTurn) = print ("Turn: " ^ Int.toString(theTurn) ^ "\n");
fun printEnergy(ENERGY) = print ("Energy: " ^ Int.toString(ENERGY) ^ "\n");

(* Grabs a line of input from the user and snips off the new line at the end *)
fun getUserCommand() = (
    let
        val s = valOf( TextIO.inputLine TextIO.stdIn )
        val userInput = String.substring(s, 0, size s - 1)
    in
        userInput
    end
);

(* Return values for different elements of the playing cards *)
fun first(x, _, _, _, _) = x;       (* Cost, int *)
fun second(_, x, _, _, _) = x;      (* Spell, bool *)
fun third(_, _, x, _, _) = x;       (* Name, string *)
fun fourth(_, _, _, x, _) = x;      (* Attack, int *)
fun fifth(_, _, _, _, x) = x;       (* Health, int *)

(* For return from playing a card, tuple value is player-hand, player-minion-list, player-energy, enemy minion list *)
fun one(x, _, _, _, _, _) = x;              (* Player-hand, card list *)
fun two(_, x, _, _, _, _) = x;              (* Player-minion-list, card list *)
fun three(_, _, x, _, _, _) = x;            (* Player-energy, int *)
fun four(_, _, _, x, _, _) = x;             (* Enemy-HP, int *)
fun five(_, _, _, _, x, _) = x;             (* Card-name & Turn-played, tuple -> string, int *)
fun six(_, _, _, _, _, x) = x;              (* Enemy minion list, card list *)

(* Return values for card name and turn played *)
fun tuple_one(x, _) = x;            (* Name of card from tuple pertaining to when it was played, string *)
fun tuple_two(_, x) = x;            (* Turn card was played from tuple pertaining to card that was played, int *)

(* Return values corresponding to player attack functionality *)
fun num_one(x, _, _, _) = x;        (* Player minion list, card list *)
fun num_two(_, x, _, _) = x;        (* Enemy minion list, card list *)
fun num_three(_, _, x, _) = x;      (* Enemy HP, int *)
fun num_four(_, _, _, x) = x;       (* Attack status of cards in play (i.e. can this card attack this turn), list of tuples of string, bool *)

(* These are used in returning values from the enemey play cards section of the enemy turn *)
fun item_one(x, _, _, _, _, _, _) = x;
fun item_two(_, x, _, _, _, _, _) = x;
fun item_three(_, _, x, _, _, _, _) = x;
fun item_four(_, _, _, x, _, _, _) = x;
fun item_five(_, _, _, _, x, _, _) = x;
fun item_six(_, _, _, _, _, x, _) = x;
fun item_seven(_, _, _, _, _, _, x) = x;

(* These are used with the enemy minion attack return values *)
fun first_item(x, _, _) = x;
fun second_item(_, x, _) = x;
fun third_item(_, _, x) = x;

(* Grabs the head of the passed in list *)
fun getHead(list) = (
    if length list = 0 then (0, false, "End of cards in hand", 0, 0)
    else hd(list)
);

(* Prints appropriate values if card is a spell *)
fun printIfSpell(card) = (
    if second(card) = true then print("\n") else print("  hp: " ^ Int.toString(fifth card) ^ "\n")
);

(* Prints the cards in the player's hand *)
fun printHand(Hand) = (
    let
        val card = getHead(Hand)
    in
        if Hand = [] then print("End of cards in hand\n")
        else (
            print("  - " ^ third(card) ^ "  cost: " ^ Int.toString(first(card)) ^ "  spell: " ^ (if second(card) = true then "Yes" else "No"));
            print("  atk: " ^ Int.toString(fourth card));
            printIfSpell(card);
            printHand(tl(Hand))
        )
    end
);

(* Returns true if the command passed in matches the name of one of the cards in list "Hand" *)
fun selectedCard(Command, Hand) = (
    let
        val card = getHead(Hand)
    in
       if (Command = third(card)) then true
       else (
            if Hand = [] then false
            else selectedCard(Command, tl(Hand))
       )
    end
);

(* Checks if the passed-in name is a spell from the list *)
fun isSpell(Command, Hand) = (
    let
        val card = getHead(Hand)
    in
        if Command = third(card) then (
            if second(card) = true then true
            else false
        )
        else (
            if Hand = [] then false
            else isSpell(Command, tl(Hand))
        )
    end
);

(* Returns a list without 'item' in the list *)
fun deleteListItem(item, list) = (
    case list of
        [] => []
        | (head::tail) => if item = third(head) then deleteListItem(item, tail)
                        else head::deleteListItem(item, tail)
);

(* Returns a copy of card specified based on name of card in a list of cards or empty otherwise *)
fun getCard(cardName, List) = (
    case List of
        [] => (0, false, "INVALID", 0, 0)
        | (head::tail) => if cardName = third(head) then head
                        else (getCard(cardName, tail))
);

(* Checks if the 'target' is in the minion list and returns the appropriate bool value to match *)
fun availableTarget(target, MinionList) = (
    if target = "enemy" orelse target = "Enemy" then true
    else (
        case MinionList of
            [] => false
            | (head::tail) => (
                if target = third(head) then true
                else (availableTarget(target, tail))
            )
    )
)

(* Takes care of damage to minions from spells *)
fun resolveMinionDamageFromSpells(Damage, Target, MinionList, Count) = (     (* Currently only resolves correctly for spell use - will need a new function for minion-v-minion damage *)
    let
        val card = ref (0, false, "INVALID", 0, 0)
        and minionList = ref MinionList
        and health = ref 0
    in
        case MinionList of
            [] => []
            | (head::tail) => (
                if Target = third(head) then (                                                      (* Find the targeted minion *)
                    if ((fifth(head) - Damage) < 0) orelse ((fifth(head) - Damage) = 0) then (      (* Does the damage kill the minion? *)
                        deleteListItem(third(head), MinionList)
                    )
                    else (                                                                          (* Minion not dead from spell, update minion HP *)
                        health := fifth(head) - Damage;
                        card := (first(head), second(head), third(head), fourth(head), !health);
                        (!card::tail)
                    )
                )
                else (
                    if Count > 0 then resolveMinionDamageFromSpells(Damage, Target, tail@[head], Count - 1)
                    else (
                        MinionList
                    )
                )
            )
    end
);

(* Returns true if the turn the minion passed in is not the same as the current turn passed in, false otherwise *)
fun checkTurnMinionPlayed(Name, TupleMinionList, Turn) = (
    case TupleMinionList of
        [] => false
        | (head::tail) => (
            if Name = tuple_one(head) then (
                if Turn <> tuple_two(head) then false
                else true
            )
            else (
                checkTurnMinionPlayed(Name, tail, Turn)
            )
        )
);

(* Returns false if valid minion and hasn't attacked yet, true otherwise *)
fun checkAttackedAlready(Name, AttackStatus) = (
    case AttackStatus of
        [] => true
        | (head::tail) => (
            if Name = tuple_one(head) then (
                if tuple_two(head) = true then true
                else false
            )
            else (
                checkAttackedAlready(Name, tail)
            )
        )
);

(* Returns a copy of the card based on the name passed in *)
(* Assumes minion is in the list passed in, otherwise will loop endlessly *)
fun getMinion(Minion, MinionList) = (
    case MinionList of 
        [] => (0, false, "INVALID", 0, 0)
        | (head::tail) => (
            if third(head) = Minion then (  (* Found the minion *)
                head
            )
            else getMinion(Minion, tail)    (* Keep searching *)
        )
);

(* Uses the minion's name and changes the attack status from false (i.e. hasn't attacked) to true (i.e. just attacked) *)
fun updateMinionAttackStatus(Minion, AttackStatus, Count) = (
    case AttackStatus of
        [] => []
        | (head::tail) => (
            if tuple_one(head) = Minion then (  (* Found the minion *)
                let
                    val attackStatus = ref tail
                    and Head = ref head
                in
                    Head := (Minion, true);
                    attackStatus := !attackStatus@[!Head];
                    !attackStatus
                end
            )
            else (
                if Count > 0 then updateMinionAttackStatus(Minion, tail@[head], Count - 1)
                else AttackStatus
            )
        )
);


(* Returns the list of cards with updated values based on damage taken - assumes minion is in minion list otherwise will loop endlessly *)
fun updateMinionList(Damage, Minion, MinionList) = (
    let
        val minion = ref (0, false, "INVALID", 0, 0)
        and health = ref 0
    in
        case MinionList of
        [] => []
        | (head::tail) => (
            if third(head) = Minion then (
                health := fifth(head) - Damage;
                minion := (first(head), second(head), third(head), fourth(head), !health);
                (!minion::tail)
            )
            else updateMinionList(Damage, Minion, tail@[head])
        )
    end
);

(* Draw a card from the deck and put it in the hand *)
fun Draw(Hand, Deck) = (
    case Deck of
    [] => (Hand, Deck)
    | (head::tail) => (
        (head::Hand, tail)
    )
);

(* Print minion names, used in printGameBoard *)
fun printMinions(minionList) = (
    case minionList of
        [] => print(" ")
        | (head::tail) => (
            print(third(head) ^ " | ");
            printMinions(tail)
        )
);

(* Prints the current game board status *)
fun printGameBoard(playerMinionList, enemyMinionList, playerHP, enemyHP) = (
    print("\n\t\t\t\tEnemy HP: " ^ Int.toString(enemyHP));
    print("\n\t---------------------------------------------------------\n\t|\t\t\t\t\t\t\t|\n\t  ");
    printMinions(enemyMinionList);
    print("\n\t|\t\t\t\t\t\t\t|\n\t---------------------------------------------------------\n\t");
    print("|\t\t\t\t\t\t\t|\n\t  ");
    printMinions(playerMinionList);
    print("\n\t|\t\t\t\t\t\t\t|\n\t---------------------------------------------------------\n");
    print("\t\t\t\tPlayer HP: " ^ Int.toString(playerHP) ^ "\n\n")
);

(* Prints out the current game state *)
fun printGameState(turn, playerMinions, enemyMinions, playerHP, enemyHP) = (
    print("Turn: " ^ Int.toString(turn) ^ "\n");
    (* print("Player Turn: " ^ Bool.toString(if turn mod 2 = 1 then true else false) ^ "\n"); *)
    print("Game Board:\n");
    printGameBoard(playerMinions, enemyMinions, playerHP, enemyHP)
);

(* Prints the cards in the player's hand *)
fun printPlayerHand(Hand) = (
    print("-------------------------\n");
    print("Cards in hand:\n");
    printHand(Hand);
    print("-------------------------\n")
)

(* Prints the player's minions in play *)
fun printPlayerMinions(MinionList) = (
    let
        val minion = getHead(MinionList)
    in
        case MinionList of
            [] => print("End of minions in play\n")
            | (head::tail) => (
                print("  - " ^ third(head) ^ "  HP: " ^ Int.toString(fifth(head)) ^ "  Atk: " ^ Int.toString(fourth(head)) ^ "\n");
                printHand(tail)
            )
    end
);

(* Prints the actual enemy minions *)
fun printEnemyMinions(MinionList) = (
    let
        val minion = getHead(MinionList)
    in
        case MinionList of
            [] => print("-------------------------\n")
            | (head::tail) => (
                print("  - " ^ third(head) ^ "  HP: " ^ Int.toString(fifth(head)) ^ "  Atk: " ^ Int.toString(fourth(head)) ^ "\n");
                printEnemyMinions(tail)
            )
    end
);

(* Prints the enemy minions in play *)
fun printEnemyMinionList(MinionList) = (
    print("-------------------------\n");
    print("Minions in play:\n");
    printPlayerMinions(MinionList);
    print("-------------------------\n")
);

(* Prints appropriate options for who to attack *)
fun printEnemyAttackOptions(MinionList) = (
    print("Intended target?\n");
    print("-------------------------\n");
    print("  - Enemy\n");
    printEnemyMinions(MinionList)
);

(* Prints from a tuple of two values: Name of card and turn played *)
fun printTuple(tupleList) = (
    case tupleList of
        [] => print("EOL\n")
        | (head::tail) => (
            print("Name: " ^ tuple_one(head) ^ "  Turn: " ^ Int.toString(tuple_two(head)) ^ "\n");
            printTuple(tail)
        )
);

(* This function manages input from player to determine if they can play a card or not, with special instructions if it is a spell card *)
fun playCard(Hand, MinionList, MinionList2, Command, Energy, Count, EnemyHP, Turn) = (
    let
        val cardChosenByPlayer = ref (0, false, "INVALID", 0, 0)
        and hand = ref Hand
        and minionList = ref MinionList     (* Player minion list *)
        and minionList2 = ref MinionList2   (* Enemy minion list *)
        and energy = ref Energy
        and count = ref Count
        and target = ref "EMPTY"
        and enemyHP = ref EnemyHP
        and turnPlayed = ref ("CARD_NAME", 0)
    in
        case Hand of
            [] => ([], MinionList, !energy, !enemyHP, !turnPlayed, MinionList2)
            | (head::tail) => (
                if Command = third(head) then (                                         (* Is the current card the selected card? *)
                    if first(head) < Energy orelse first(head) = Energy then (          (* Does the player have enough energy? *)
                        if second(head) = true then (                                   (* Is the card a spell? *)
                            print("Play " ^ third(head) ^ " on whom?\n");
                            printEnemyAttackOptions(!minionList2);
                            target := getUserCommand();
                            if availableTarget(!target, !minionList2) = true then (     (* Did they choose a valid, case-sensitive target? *)
                                if !target = "enemy" orelse !target = "Enemy" then (    (* Do they want to target and damage the enemy avatar? *)
                                    enemyHP := !enemyHP - fourth(head);
                                    hand := deleteListItem(third(head), Hand);
                                    energy := !energy - first(head);
                                    print("Played " ^ third(head) ^ " on the enemy!\n");
                                    (!hand, MinionList, !energy, !enemyHP, !turnPlayed, MinionList2)
                                )
                                else (                                                  (* Taget and damage an enemy minion *)
                                    hand := deleteListItem(third(head), Hand);
                                    energy := !energy - first(head);
                                    minionList2 := resolveMinionDamageFromSpells(fourth(head), !target, !minionList2, length(!minionList2));
                                    print("Played " ^ third(head) ^ " on " ^ !target ^ "!\n");
                                    (!hand, !minionList, !energy, !enemyHP, !turnPlayed, !minionList2)
                                )
                            )
                            else (                                                      (* Spell card in hand and found, but invalid target - let them try again *)
                                print("Invalid spell target\n");
                                (Hand, MinionList, Energy, EnemyHP, !turnPlayed, MinionList2)
                            )

                        )
                        else (                                                          (* Card is a playable minion from their hand *)
                            cardChosenByPlayer := head;
                            hand := deleteListItem(third(head), Hand);
                            minionList := !cardChosenByPlayer::MinionList;
                            print("Played " ^ third(head) ^ "!\n");
                            energy := !energy - first(head);
                            turnPlayed := (third(head), Turn);
                            (!hand, !minionList, !energy, !enemyHP, !turnPlayed, MinionList2)
                        )
                    )
                    else (                                                              (* Not enough energy to play desired card *)
                        print("Not enough energy...\n");
                        (Hand, MinionList, Energy, EnemyHP, !turnPlayed, MinionList2)
                    )
                )
                else (
                    if !count > 0 then playCard(tail@[head], MinionList, MinionList2, Command, Energy, !count - 1, EnemyHP, Turn)
                    else (
                        print("Invalid name...\n");
                        (Hand, MinionList, Energy, EnemyHP, !turnPlayed, MinionList2)
                    )
                )
            )
    end
);

(* This is called from resolvePlayerMinionAttack and assumes both "AttackingMinion" and "Target" are both in their respective lists already *)
(* This will loop endlessly if either "AttackingMinion" or "Target" are not in their respective lists when called *)
fun resolveMinionDamage(AttackingMinion, Target, MinionList, OpponentMinionList) = (
    case MinionList of
        [] => (MinionList, OpponentMinionList)
        | (head1::tail1) => (
            if third(head1) = AttackingMinion then (
                case OpponentMinionList of 
                    [] => (MinionList, OpponentMinionList)
                    | (head2::tail2) => (
                        if third(head2) = Target then (
                            let
                                val attacker = ref (0, false, "INVALID", 0, 0)
                                and target = ref (0, false, "INVALID", 0, 0)
                                and playerMinionList = ref []
                                and opponentMinionList = ref []
                                and attackerHealth = ref 0
                                and targetHealth = ref 0
                            in
                                attacker := getCard(AttackingMinion, MinionList);
                                target := getCard(Target, OpponentMinionList);
                                attackerHealth := fifth(!attacker) - fourth(!target);
                                targetHealth := fifth(!target) - fourth(!attacker);
                                attacker := (first(!attacker), second(!attacker), third(!attacker), fourth(!attacker), !attackerHealth);
                                target := (first(!target), second(!target), third(!target), fifth(!attacker), !targetHealth);
                                if fifth(!target) = 0 orelse fifth(!target) < 0 then (                  (* Opponent minion died, update appropriate minion list *)
                                    opponentMinionList := deleteListItem(third(!target), OpponentMinionList)
                                )
                                else (                                                                  (* Opponent minion took damage, upate lists appropriately *)
                                    opponentMinionList := updateMinionList(fourth(!attacker), third(!target), OpponentMinionList)
                                );
                                if fifth(!attacker) = 0 orelse fifth(!attacker) < 0 then (              (* User minion died, update appropriate minion list *)
                                    playerMinionList := deleteListItem(third(!attacker), MinionList)
                                )
                                else (                                                                  (* User minion took damage, update list appropriately *)
                                    playerMinionList := updateMinionList(fourth(!target), third(!attacker), MinionList)
                                );
                                (!playerMinionList, !opponentMinionList)
                            end
                        )
                        else resolveMinionDamage(AttackingMinion, Target, MinionList, tail2@[head2])
                    )
            )
            else resolveMinionDamage(AttackingMinion, Target, tail1@[head1], OpponentMinionList)
        )
);

(* Takes care of targeting and damage done by a player minion *)
fun resolvePlayerMinionAttack(AttackingMinion, Target, MinionList, OpponentMinionList, OpponentHP, MinionAttackStatus) = (
    let
        val opponentHP = ref OpponentHP
        and minionAttackStatus = ref MinionAttackStatus
        and minionList = ref MinionList
        and opponentMinionList = ref OpponentMinionList
        and updatedMinionLists = ref ([], [])
    in
        if selectedCard(Target, OpponentMinionList) = true orelse Target = "Enemy" orelse Target = "enemy" then (   (* Valid target to attack *)
            if Target = "Enemy" orelse Target = "enemy" then (                                                      (* Target is the enemy *)
                print(AttackingMinion ^ " attacked the enemy!\n");
                opponentHP := OpponentHP - fourth(getMinion(AttackingMinion, MinionList));
                minionAttackStatus := updateMinionAttackStatus(AttackingMinion, MinionAttackStatus, length(MinionAttackStatus));
                (MinionList, OpponentMinionList, !opponentHP, !minionAttackStatus)
            )
            else (                                                                                                  (* Target is an enemy minion in play *)
                print(AttackingMinion ^ " attacked " ^ Target ^ "!\n");
                updatedMinionLists := resolveMinionDamage(AttackingMinion, Target, MinionList, OpponentMinionList);
                minionList := tuple_one(!updatedMinionLists);
                opponentMinionList := tuple_two(!updatedMinionLists);
                minionAttackStatus := updateMinionAttackStatus(AttackingMinion, MinionAttackStatus, length(MinionAttackStatus));
                (!minionList, !opponentMinionList, OpponentHP, !minionAttackStatus)
            )
        )
        else (                                                                                                      (* Invalid attack target, try again *)
            print("Invalid target\n");
            (MinionList, OpponentMinionList, OpponentHP, MinionAttackStatus)
        )
    end
);

(* This function is run at the end of the player turn and allows for minions to attack again once on player turn *)
fun refreshAttackStatus(AttackStatus, Count) = (
    case AttackStatus of
        [] => []
        | (head::tail) => (
            if Count < 0 orelse Count = 0 then (
                AttackStatus
            )
            else (
                let
                    val cardAttackStatus = ref ("NAME", false)
                in
                    cardAttackStatus := (tuple_one(head), false);
                    refreshAttackStatus(tail@[!cardAttackStatus], Count - 1)
                end
            )
        )
);

(* Checks passed HP to determine if that player is dead or not *)
fun isPlayerDead(playerHP) = (
    if playerHP = 0 orelse playerHP < 0 then true
    else false
)

(*==================================================================================================*)
(*==================================================================================================*)

(* Returns PlayerHP and PlayerMinionList updated accordingly after being damaged by the spell passed in *)
fun enemyResolveSpellDamage(Damage, PlayerHP, PlayerMinionList, Count) = (
    case PlayerMinionList of
        [] => (
            print("Enemy dealt " ^ Int.toString(Damage) ^ " to your health!\n");
            (PlayerHP - Damage, PlayerMinionList)
        )
        | (head::tail) => (
            if fifth(head) = Damage orelse fifth(head) < Damage then (          (* Player Minion dies from spell damage *)
                print("Enemy dealt " ^ Int.toString(Damage) ^ " damage to " ^ third(head) ^ " with the spell!\n");
                (PlayerHP, deleteListItem(third(head), PlayerMinionList))
            )
            else (                                                              (* Check the rest of the minions on the player's side *)
                if Count > 0 then (
                    enemyResolveSpellDamage(Damage, PlayerHP, tail@[head], Count - 1)
                )
                else (                                                          (* Player takes the hit instead *)
                    print("Enemy dealt " ^ Int.toString(Damage) ^ " to your health!\n");
                    (PlayerHP - Damage, PlayerMinionList)
                )
            )
        )
)

(* Takes care of targeting and applying damage for an enemy minion attack *)
fun resolveEnemyMinionAttackDamage(Attacker, PlayerMinionList, PlayerHP, Count, EnemyMinionList) = (
    let
        val attacker = ref Attacker
    in
        case PlayerMinionList of
            [] => (                                                                             (* No minions to attack, just hit player HP directly *)
                print(third(Attacker) ^ " attacked you directly for " ^ Int.toString(fourth(Attacker)) ^ " damage!\n");
                (EnemyMinionList@[Attacker], PlayerMinionList, PlayerHP - fourth(Attacker))
            )
            | (head::tail) => (                                                                 (* Target a minion if damage kills minion from attack, otherwise just hit player *)
                if fifth(Attacker) = fourth(head) orelse fifth(Attacker) > fourth(head) then ( (* Attack kills the minion, so attack it *)
                    print(third(Attacker) ^ " attacked " ^ third(head) ^ " for " ^ Int.toString(fourth(Attacker)) ^ " damage!\n");
                    attacker := (first(Attacker), second(Attacker), third(Attacker), fourth(Attacker), fifth(Attacker) - fourth(head));
                    if fourth(!attacker) = 0 orelse fourth(!attacker) < 0 then (
                        (deleteListItem(third(!attacker), EnemyMinionList), deleteListItem(third(head), PlayerMinionList), PlayerHP)
                    )
                    else (
                        (EnemyMinionList@[!attacker], deleteListItem(third(head), PlayerMinionList), PlayerHP)                   
                    )
                )
                else (                                                                          (* See if there is a minion on the opponenet's side of the field that dies to minion's attack *)
                    if Count > 0 then (
                        resolveEnemyMinionAttackDamage(Attacker, tail@[head], PlayerHP, Count - 1, EnemyMinionList)
                    )
                    else (                                                                      (* No minions that die to attack, so just target player HP directly *)
                        print(third(Attacker) ^ " attacked you directly for " ^ Int.toString(fourth(Attacker)) ^ " damage!\n");
                        (EnemyMinionList@[Attacker], PlayerMinionList, PlayerHP - fourth(Attacker))
                    )
                )
            )
    end
);



(* Returns copy of the first card with the highest energy value in the hand which is also equal to or less than the current player energy *)
fun enemyGetPlayCard(EnemyHand, EnemyEnergy, LargestLegalValueCard) = (
    case EnemyHand of
        [] =>  LargestLegalValueCard
        | (head::tail) => (
            if first(head) = EnemyEnergy orelse first(head) < EnemyEnergy then (
                if first(head) > first(LargestLegalValueCard) then (
                    enemyGetPlayCard(tail, EnemyEnergy, head)
                )
                else (
                    LargestLegalValueCard
                )
            )
            else (
                enemyGetPlayCard(tail, EnemyEnergy, LargestLegalValueCard)
            )
        )
)

(* Recursively plays the highest energy value minion possible if able *)
fun enemyPlayCards(EnemyHand, EnemyMinionList, EnemyEnergy, PlayerHP, PlayerMinionList, Count, Turn, CardsTurnPlayed) = (
    let
        val playerHP = ref PlayerHP
        and playThisCard = ref (0, false, "INVALID", 0, 0)
        and spellAttackResults = ref (0, [])
        and cardsTurnPlayed = ref CardsTurnPlayed
    in
        case EnemyHand of
            [] => (EnemyHand, EnemyMinionList, EnemyEnergy, PlayerHP, PlayerMinionList, CardsTurnPlayed)
            | (head::tail) => (
                if first(head) = EnemyEnergy orelse first(head) < EnemyEnergy then (        (* Finding a minion that can be played *)
                    playThisCard := enemyGetPlayCard(EnemyHand, EnemyEnergy, head);         (* Found the optimal minion to be played from hand *)
                    if second(!playThisCard) = true then (                                  (* Spell being played - need to choose target *)
                        print("Enemy played " ^ third(!playThisCard) ^ "!\n");
                        spellAttackResults := enemyResolveSpellDamage(fourth(!playThisCard), PlayerHP, PlayerMinionList, length(PlayerMinionList));
                        (deleteListItem(third(!playThisCard), EnemyHand), EnemyMinionList, EnemyEnergy - first(!playThisCard), tuple_one(!spellAttackResults), tuple_two(!spellAttackResults), CardsTurnPlayed)
                    )
                    else (                                                                  (* Minion being played *)
                        print("Enemy played " ^ third(!playThisCard) ^ "!\n");
                        cardsTurnPlayed := (!cardsTurnPlayed)@[(third(!playThisCard), Turn)];
                        (deleteListItem(third(!playThisCard), EnemyHand), (!playThisCard)::EnemyMinionList, EnemyEnergy - first(!playThisCard), PlayerHP, PlayerMinionList, !cardsTurnPlayed)
                    )
                    
                )
                else (
                    if Count > 0 then (
                        enemyPlayCards(tail@[head], EnemyMinionList, EnemyEnergy, PlayerHP, PlayerMinionList, Count - 1, Turn, CardsTurnPlayed)  (* Cycle through the rest of the cards in hand *)
                    )
                    else (                                                                  (* No minions values were less than or equal to the current energy cost *)
                        (EnemyHand, EnemyMinionList, EnemyEnergy, PlayerHP, PlayerMinionList, CardsTurnPlayed)
                    )
                )
            )
    end
);


(* The first minion in the list of enemy minions in play attacks and then goes to the end of the list *)
fun enemyMinionAttack(EnemyMinionList, PlayerMinionList, PlayerHP, CardsPlayedOnTurns, Turn, Count) = (
    let
        val returnResults = ref ([], [], 0)
    in
        case EnemyMinionList of
            [] => (EnemyMinionList, PlayerMinionList, PlayerHP)
            | (head::tail) => (
                if checkTurnMinionPlayed(third(head), CardsPlayedOnTurns, Turn) = true then (   (* Make sure not trying to attack on the same turn the minion was played *)
                    if Count > 0 then (                                                         (* While it hasn't checked all the enemy minions, keep doing this *)
                        enemyMinionAttack(tail@[head], PlayerMinionList, PlayerHP, CardsPlayedOnTurns, Turn, Count - 1)
                    )
                    else (                                                                      (* Exhausted all minions in play; none valid so just return values *)
                        (EnemyMinionList, PlayerMinionList, PlayerHP)
                    )
                )
                else (                                                                          (* Valid minion and not attacking on the turn it was played *)
                    returnResults := resolveEnemyMinionAttackDamage(head, PlayerMinionList, PlayerHP, length(PlayerMinionList), tail);
                    !returnResults
                )
            )
    end
);

(*==================================================================================================*)
(*==================================================================================================*)

(* Next ~15 lines of code courtesy of stackoverflow *)
(* select([5,6,7,8,9], 2) = (7, [5,6,8,9]) *)
fun select (y::xs, 0) = (y, xs)
  | select (x::xs, i) = let val (y, xs') = select (xs, i-1) in (y, x::xs') end
  | select (_, i) = raise Fail ("Short by " ^ Int.toString i ^ " elements.");

(* Recreates a list in random order by removing elements in random positions *)
fun shuffle xs =
    let fun rtake [] _ = []
          | rtake ys max =
            let val (y, ys') = select (ys, Random.randRange(0, max - 1) seed)
            in y :: rtake ys' (max-1)
            end
    in rtake xs (length xs) end;


(* Main game loop - this is where the game takes place *)
fun playGame(GameOver, pHand, pDeck, eHand, eDeck) = (
let
    val gameOver = ref GameOver
    and turn = ref 0
    and energy = ref 0
    and playerEnergy = ref 0
    and playerTurn = ref false
    and playerHP = ref 30
    and playerHand = ref pHand
    and playerMinionList = ref []
    and playerDeck = ref pDeck
    and enemyEnergy = ref 0
    and enemyTurn = ref false
    and enemyHP = ref 30
    and enemyHand = ref eHand
    and enemyMinionList = ref []
    and enemyDeck = ref eDeck
    and updatedPHandPMinions = ref ([], [], 0, 0, ("EMPTY", 0), []) (* Tuple | Hand, MinionList, PEnergy, EnemyHP, tuple of card names and turns played, EnemyMinionList *)
    and updatedEHandEMinions = ref ([], [], 0, 0, [], []) (* (EnemyHand, EnemyMinionList, EnemyEnergy, PlayerHP, PlayerMinionList, AttackStatus, CardsPlayedOnTurns) *)
    and cardsPlayedOnTurns = ref []
    and attackStatus = ref []           (* Tuple | name of card, boolean flag - attacked this turn = true | string, bool *)
    and postPlayerMinionAttack = ref ([], [], 0, []) (* Tuple | (MinionList, OpponentMinionList, OpponentHP, MinionAttackStatus) *)
    and postEnemyMinionAttack = ref ([], [], 0) (* Tuple | Same as directly one above *)
    and updatingHandAndDeck = ref ([], [])  (* Tuple | deck and hand: card list, card list *)

in
    updatingHandAndDeck := Draw(!playerHand, !playerDeck);
    playerHand := tuple_one(!updatingHandAndDeck);
    playerDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!playerHand, !playerDeck);
    playerHand := tuple_one(!updatingHandAndDeck);
    playerDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!playerHand, !playerDeck);
    playerHand := tuple_one(!updatingHandAndDeck);
    playerDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!playerHand, !playerDeck);
    playerHand := tuple_one(!updatingHandAndDeck);
    playerDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!enemyHand, !enemyDeck);
    enemyHand := tuple_one(!updatingHandAndDeck);
    enemyDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!enemyHand, !enemyDeck);
    enemyHand := tuple_one(!updatingHandAndDeck);
    enemyDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!enemyHand, !enemyDeck);
    enemyHand := tuple_one(!updatingHandAndDeck);
    enemyDeck := tuple_two(!updatingHandAndDeck);

    updatingHandAndDeck := Draw(!enemyHand, !enemyDeck);
    enemyHand := tuple_one(!updatingHandAndDeck);
    enemyDeck := tuple_two(!updatingHandAndDeck);
    
    while !gameOver <> true do (
        turn := setTurn(!turn);
        playerEnergy := setEnergy(!turn);
        enemyEnergy := setEnergy(!turn);
        printGameState(!turn, !playerMinionList, !enemyMinionList, !playerHP, !enemyHP);
        playerTurn := true;
        print("Drawing. . .\n");
        updatingHandAndDeck := Draw(!playerHand, !playerDeck);
        playerHand := tuple_one(!updatingHandAndDeck);
        playerDeck := tuple_two(!updatingHandAndDeck);
        while !playerTurn = true do (
            print("Energy: " ^ Int.toString(!playerEnergy) ^ "\n");
            print("Player Options:\n   'quit'\n   'end'\n   'play'\n   'attack'\n");
            let
                val choice = getUserCommand()
                and command = ref "EMPTY"
                and target = ref "EMPTY"
            in
                case choice of
                    "quit" => (print("Quitting. . .\n"); gameOver := true; playerTurn := false)     (* May need to change this to just hard quit... *)
                    | "end" => (print("Turn Over\n\n"); playerTurn := false)
                    | "play" => (
                        printPlayerHand(!playerHand);
                        print("Choose a card to play: ");
                        command := getUserCommand();
                        print("\n");
                        updatedPHandPMinions := playCard(!playerHand, !playerMinionList, !enemyMinionList, !command, !playerEnergy, (length (!playerHand)), !enemyHP, !turn);
                        playerHand := one(!updatedPHandPMinions);
                        playerMinionList := two(!updatedPHandPMinions);
                        playerEnergy := three(!updatedPHandPMinions);
                        enemyHP := four(!updatedPHandPMinions);
                        cardsPlayedOnTurns := !cardsPlayedOnTurns@[five(!updatedPHandPMinions)];
                        enemyMinionList := six(!updatedPHandPMinions);
                        if (selectedCard(!command, !playerMinionList) = true) then (
                            attackStatus := (!attackStatus)@[(!command, false)]
                        )
                        else ()
                     )
                    | "attack" => (
                        if length(!playerMinionList) > 0 then (                                     (* Making sure they actually have a minion on the field *)
                            print("Attack using which minion?\n");
                            printPlayerMinions(!playerMinionList);
                            command := getUserCommand();
                            if selectedCard(!command, !playerMinionList) = true then (              (* Did the user type a valid minion, case-sensitive? *)
                                if checkTurnMinionPlayed(!command, !cardsPlayedOnTurns, !turn) = true then (    (* Make sure not trying to attack on the same turn the minion was played *)
                                    print("Minions cannot attack on the turn they were played\n")
                                )
                                else (                                                              (* Valid minion and not attacking on the turn it was played *)
                                    if checkAttackedAlready(!command, !attackStatus) = true then (  (* Check to see if minion has already attacked *)
                                        print("Minions can only attack once per turn\n")
                                    )
                                    else (                                                          (* Valid minion, which hasn't attacked yet *)
                                        printEnemyAttackOptions(!enemyMinionList);
                                        target := getUserCommand();
                                        postPlayerMinionAttack := resolvePlayerMinionAttack(!command, !target, !playerMinionList, !enemyMinionList, !enemyHP, !attackStatus);
                                        playerMinionList := num_one(!postPlayerMinionAttack);
                                        enemyMinionList := num_two(!postPlayerMinionAttack);
                                        enemyHP := num_three(!postPlayerMinionAttack);
                                        attackStatus := num_four(!postPlayerMinionAttack)
                                    )
                                )
                            )
                            else (                                                                  (* Didn't choose a valid minion option *)
                                print("Invalid minion option\n")
                            )
                        )
                        else (                                                              (* No minion = cannot attack *)
                            print("You need a minion in play in order to attack\n")
                        )
                    )
                    | _ => (print("Invalid choice\n"));
            if isPlayerDead(!playerHP) orelse isPlayerDead(!enemyHP) then (
                if isPlayerDead(!playerHP) then print("\n\nYOU LOSE. . .\n\n")
                else if isPlayerDead(!enemyHP) then print("\n\nYOU WIN!!!\n\n")
                else ();
                playerTurn := false;
                gameOver := true
            )
            else if choice = "quit" then (
                playerTurn := false;
                gameOver := true
            )
            else (gameOver := false)
            end
        );
        attackStatus := refreshAttackStatus(!attackStatus, length(!attackStatus));
        !playerTurn;

        if !gameOver = true then enemyTurn := false
        else enemyTurn := true;

        while !enemyTurn = true do (
            let
                val count1 = ref 0
                and count2 = ref 0
            in

                updatingHandAndDeck := Draw(!enemyHand, !enemyDeck);
                enemyHand := tuple_one(!updatingHandAndDeck);
                enemyDeck := tuple_two(!updatingHandAndDeck);
                count1 := length(!enemyHand);
                count2 := length(!enemyMinionList);
            
                (* Play cards *)
                while !count1 > 0 do (
                    updatedEHandEMinions := enemyPlayCards(!enemyHand, !enemyMinionList, !enemyEnergy, !playerHP, !playerMinionList, length(!enemyHand), !turn, !cardsPlayedOnTurns);
                    enemyHand := one(!updatedEHandEMinions);
                    enemyMinionList := two(!updatedEHandEMinions);
                    enemyEnergy := three(!updatedEHandEMinions);
                    playerHP := four(!updatedEHandEMinions);
                    playerMinionList := five(!updatedEHandEMinions);
                    cardsPlayedOnTurns := six(!updatedEHandEMinions);
                    count1 := !count1 - 1
                );
                !count1;

                (* Attack with all monsters *)
                while !count2 > 0 do (                       (* (EnemyMinionList, PlayerMinionList, PlayerHP, CardsPlayedOnTurns, Turn, Count) *)
                    postEnemyMinionAttack := enemyMinionAttack(!enemyMinionList, !playerMinionList, !playerHP, !cardsPlayedOnTurns, !turn, length(!enemyMinionList));
                    enemyMinionList := first_item(!postEnemyMinionAttack);
                    playerMinionList := second_item(!postEnemyMinionAttack);
                    playerHP := third_item(!postEnemyMinionAttack);
                    count2 := !count2 - 1
                );
                !count2;

                enemyTurn := false;
                if isPlayerDead(!playerHP) orelse isPlayerDead(!enemyHP) then (
                    if isPlayerDead(!playerHP) then print("\n\nYOU LOSE. . .\n\n")
                    else if isPlayerDead(!enemyHP) then print("\n\nYOU WIN!!!\n\n")
                    else ();
                    enemyTurn := false;
                    gameOver := true
                )
                else (gameOver := false)
            end
        );
        !enemyTurn;
        
        attackStatus := refreshAttackStatus(!attackStatus, length(!attackStatus))        
    );
    !gameOver;
    if !gameOver then print("\n\n\n--------------------\n   Game Over\n--------------------\n\n\n")
    else print("\n\n\n--------------------\n   .Game Over.\n--------------------\n\n\n")
end
);

print(" ");
print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
print "Welcome to MLStone, test your skill! Best of luck!\n\n";

(* Main game loop *)
playerDeck := shuffle(!playerDeck);
enemyDeck := shuffle(!enemyDeck);
playGame(gameOver, playerHand, !playerDeck, enemyHand, !enemyDeck);

OS.Process.exit OS.Process.success;
