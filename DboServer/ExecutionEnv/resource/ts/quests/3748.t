CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3748;
	title = 374802;

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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 374820;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 15;
				cct = 1;
				crt = 0;
				r = "40.000000";
				x = "-4400.000000";
				z = "-4832.000000";
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
				conv = 374809;
				ctype = 1;
				idx = 1163101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 374808;
				area = 374801;
				goal = 374804;
				m0fz = "-4832.000000";
				m0widx = 15;
				scitem = -1;
				sort = 374805;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "-4400.000000";
				m0ttip = 374815;
				rwd = 100;
				taid = 1;
				title = 374802;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 374814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 374801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1163101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 374807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "1163101;";
			}
			CDboTSCheckClrQst
			{
				and = "3747;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 374808;
			gtype = 1;
			oklnk = 2;
			area = 374801;
			goal = 374804;
			sort = 374805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 374802;
		}
	}
}

