CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 623;
	title = 62302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 62308;
				gtype = 1;
				area = 62301;
				goal = 62304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 62305;
				stype = 2;
				taid = 1;
				title = 62302;
			}
			CDboTSActNPCConv
			{
				conv = 62309;
				ctype = 1;
				idx = 4511102;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 62308;
			gtype = 1;
			oklnk = 2;
			area = 62301;
			goal = 62304;
			sort = 62305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 62302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 62314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 62301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3143105;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 62307;
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
				minlvl = 21;
			}
			CDboTSCheckClrQst
			{
				and = "622;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511102;";
			}
		}
	}
}

