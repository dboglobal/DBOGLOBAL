CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 658;
	title = 65802;

	CNtlTSGroup
	{
		gid = 0;

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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 27;
			}
			CDboTSClickNPC
			{
				npcidx = "3611104;";
			}
			CDboTSCheckClrQst
			{
				and = "793;";
				flink = 1;
				flinknextqid = "794;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 65809;
				ctype = 1;
				idx = 3611104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 65808;
				m1fx = "6308.000000";
				m2fz = "832.000000";
				m1fy = "0.000000";
				sort = 65805;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 65801;
				goal = 65804;
				m0fz = "1011.000000";
				m0widx = 1;
				m1ttip = 65816;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "698.000000";
				m2fx = "5475.000000";
				m2ttip = 65817;
				m0fx = "6326.000000";
				m0ttip = 65815;
				title = 65802;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 65808;
			gtype = 2;
			oklnk = 2;
			area = 65801;
			goal = 65804;
			sort = 65805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 65802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 65814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 65801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131102;";
			}
		}
	}
}

