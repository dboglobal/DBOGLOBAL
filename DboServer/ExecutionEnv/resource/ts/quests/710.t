CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 710;
	title = 71002;

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
				conv = 71009;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 71008;
				gtype = 1;
				area = 71001;
				goal = 71004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 71005;
				stype = 2;
				taid = 1;
				title = 71002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 71007;
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
				minlvl = 24;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
			CDboTSCheckClrQst
			{
				and = "709;";
				flink = 1;
				flinknextqid = "739;";
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 71008;
			gtype = 1;
			oklnk = 2;
			area = 71001;
			goal = 71004;
			sort = 71005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 71002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 71014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 71001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332102;";
			}
		}
	}
}

