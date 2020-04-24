CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 20;
	title = 2002;

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
				conv = 2009;
				ctype = 1;
				idx = 4181101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 2008;
				gtype = 1;
				area = 2001;
				goal = 2004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 2005;
				stype = 1;
				taid = 1;
				title = 2002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 2007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckPCCls
			{
				clsflg = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4181101;";
			}
			CDboTSCheckClrQst
			{
				and = "19;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 2008;
			gtype = 1;
			oklnk = 2;
			area = 2001;
			goal = 2004;
			sort = 2005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 2002;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 2014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 2001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7091101;";
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

