CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 18;
	title = 1802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 181;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 1808;
				gtype = 1;
				area = 1801;
				goal = 1804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 1805;
				stype = 2;
				taid = 1;
				title = 1802;
			}
			CDboTSActNPCConv
			{
				conv = 1809;
				ctype = 1;
				idx = 2413101;
				taid = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1807;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "2413101;";
			}
			CDboTSCheckClrQst
			{
				and = "17;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1808;
			gtype = 1;
			oklnk = 2;
			area = 1801;
			goal = 1804;
			sort = 1805;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 1802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 1814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 1801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061201;";
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
	}
}

