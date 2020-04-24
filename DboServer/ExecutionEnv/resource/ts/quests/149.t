CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 149;
	title = 14902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 4;
			idx = 1493;
			lilnk = 255;
			oklnk = 100;
			prelnk = "3;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "8031101;";
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
				conv = 14909;
				ctype = 1;
				idx = 2804101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 14908;
				gtype = 1;
				area = 14901;
				goal = 14904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 14905;
				stype = 1;
				taid = 1;
				title = 14902;
			}
		}
		CDboTSContReward
		{
			canclnk = 3;
			cid = 100;
			rwdzeny = 0;
			desc = 14914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 14901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;4;";
			usetbl = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 14907;
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
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "2804101;";
			}
			CDboTSCheckClrQst
			{
				and = "148;";
				flink = 1;
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
			cont = 14908;
			gtype = 1;
			oklnk = 2;
			area = 14901;
			goal = 14904;
			sort = 14905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 14902;
		}
	}
}

