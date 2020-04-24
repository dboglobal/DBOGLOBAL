CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 200;
	title = 20002;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 20008;
			gtype = 1;
			oklnk = 2;
			area = 20001;
			goal = 20004;
			sort = 20005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 20002;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 20009;
				ctype = 1;
				idx = 3221203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 20008;
				gtype = 1;
				area = 20001;
				goal = 20004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 20005;
				stype = 1;
				taid = 1;
				title = 20002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 20007;
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
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "3221203;";
			}
			CDboTSCheckClrQst
			{
				and = "111;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 20014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 20001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
		}
	}
}

