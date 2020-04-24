CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1723;
	title = 172302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 172308;
			gtype = 2;
			oklnk = 2;
			area = 172301;
			goal = 172304;
			sort = 172305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 172302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 172314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 172301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111102;";
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
			stdiag = 172307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSCheckClrQst
			{
				and = "1706;1720;";
				flink = 1;
				flinknextqid = "1724;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6111102;";
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
				conv = 172309;
				ctype = 1;
				idx = 6111102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 9;
				eitype = 0;
				idx0 = 2613200;
				cnt0 = 7;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 6013200;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 172308;
				m1fx = "-587.000000";
				m1fy = "0.000000";
				sort = 172305;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1227.000000";
				grade = 132203;
				m0fx = "-484.000000";
				m0ttip = 172315;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 172302;
				gtype = 2;
				area = 172301;
				goal = 172304;
				m0fz = "-1274.000000";
				m0widx = 1;
				m1ttip = 172316;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

