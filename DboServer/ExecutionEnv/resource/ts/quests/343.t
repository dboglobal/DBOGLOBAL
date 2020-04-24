CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 343;
	title = 34302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 34309;
				ctype = 1;
				idx = 4511208;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 34308;
				gtype = 1;
				area = 34301;
				goal = 34304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 34305;
				stype = 2;
				taid = 1;
				title = 34302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 34307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "4511208;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "344;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 34308;
			gtype = 1;
			oklnk = 2;
			area = 34301;
			goal = 34304;
			sort = 34305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 34302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 34314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 34301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511505;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

