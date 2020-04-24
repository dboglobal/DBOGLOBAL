CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 163;
	title = 16302;

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
				conv = 16309;
				ctype = 1;
				idx = 5312101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 16308;
				gtype = 1;
				area = 16301;
				goal = 16304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 16305;
				stype = 1;
				taid = 1;
				title = 16302;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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
			cont = 16308;
			gtype = 1;
			oklnk = 2;
			area = 16301;
			goal = 16304;
			sort = 16305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 16302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 16314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 16301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 16307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckClrQst
			{
				and = "162;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5312101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

