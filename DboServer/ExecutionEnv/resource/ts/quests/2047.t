CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 2047;
	title = 204702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 204707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "7571109;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 10010000;
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
			cont = 204708;
			gtype = 2;
			oklnk = 2;
			area = 204701;
			goal = 204704;
			sort = 204705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 204702;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 204714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 204701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7571109;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 204709;
				ctype = 1;
				idx = 7571109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 204708;
				gtype = 2;
				area = 204701;
				goal = 204704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 204705;
				stype = 64;
				taid = 1;
				title = 204702;
			}
		}
	}
}

