CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 211;
	title = 21102;

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
				conv = 21109;
				ctype = 1;
				idx = 8411103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 21108;
				gtype = 1;
				area = 21101;
				goal = 21104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 21105;
				stype = 64;
				taid = 1;
				title = 21102;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "8411103;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				not = 0;
				or = "210;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 21108;
			gtype = 1;
			oklnk = 2;
			area = 21101;
			goal = 21104;
			sort = 21105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 21102;
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
			desc = 21114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 21101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1111104;";
			}
		}
	}
}

