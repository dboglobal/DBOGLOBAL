CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 597;
	title = 59702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 59714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 59701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4261106;";
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 59707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "4261106;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 59709;
				ctype = 1;
				idx = 4261106;
				taid = 2;
			}
			CDboTSActOpenWindow
			{
				show = 1;
				taid = 3;
				tblidx = 20004;
				wtype = 5;
			}
			CDboTSActRegQInfo
			{
				cont = 59708;
				gtype = 2;
				area = 59701;
				goal = 59704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 59705;
				stype = 64;
				taid = 1;
				title = 59702;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSParty
			{
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 59708;
			gtype = 2;
			oklnk = 2;
			area = 59701;
			goal = 59704;
			sort = 59705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 59702;
		}
	}
}

