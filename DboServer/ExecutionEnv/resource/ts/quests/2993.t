CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2993;
	title = 299302;

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
				conv = 299309;
				ctype = 1;
				idx = 4142113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 299308;
				m1fx = "-1265.000000";
				m1fy = "0.000000";
				sort = 299305;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2000.000000";
				grade = 132203;
				m0fx = "-1129.000000";
				m0ttip = 299315;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 299302;
				gtype = 2;
				area = 299301;
				goal = 299304;
				m0fz = "-1756.000000";
				m0widx = 1;
				m1ttip = 299316;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "4142113;";
			}
			CDboTSCheckClrQst
			{
				and = "2992;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 299308;
			gtype = 2;
			oklnk = 2;
			area = 299301;
			goal = 299304;
			sort = 299305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 299302;
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
			desc = 299314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 299301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1402301;";
			}
		}
	}
}

