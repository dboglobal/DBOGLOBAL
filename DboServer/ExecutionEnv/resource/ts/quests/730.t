CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 730;
	title = 73002;

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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 73009;
				ctype = 1;
				idx = 3143105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 73008;
				m1fx = "4641.000000";
				m1fy = "0.000000";
				sort = 73005;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-67.000000";
				grade = 132203;
				m0fx = "4641.000000";
				m0ttip = 73015;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 73002;
				gtype = 2;
				area = 73001;
				goal = 73004;
				m0fz = "-67.000000";
				m0widx = 1;
				m1ttip = 73016;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 73008;
			gtype = 2;
			oklnk = 2;
			area = 73001;
			goal = 73004;
			sort = 73005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 73002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 73014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 73001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3174102;";
			}
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
			stdiag = 73007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "889;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3143105;";
			}
		}
	}
}

