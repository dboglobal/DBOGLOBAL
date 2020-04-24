CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1577;
	title = 157702;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 157707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "3241102;";
			}
			CDboTSCheckClrQst
			{
				and = "1536;";
				flink = 1;
				flinknextqid = "1581;";
				not = 0;
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
				conv = 157709;
				ctype = 1;
				idx = 3241102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 5651100;
				cnt0 = 5;
				cnt2 = 5;
				ectype = -1;
				etype = 0;
				idx1 = 5652100;
				taid = 3;
				esctype = 0;
				idx2 = 5653100;
			}
			CDboTSActRegQInfo
			{
				cont = 157708;
				m1fx = "-1532.000000";
				m2fz = "1220.000000";
				m1fy = "0.000000";
				sort = 157705;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 157701;
				goal = 157704;
				m0fz = "1224.000000";
				m0widx = 1;
				m1ttip = 157716;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1247.000000";
				m2fx = "-1460.000000";
				m2ttip = 157717;
				m0fx = "-1545.000000";
				m0ttip = 157715;
				title = 157702;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 157714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 157701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3241102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 157708;
			gtype = 2;
			oklnk = 2;
			area = 157701;
			goal = 157704;
			sort = 157705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 157702;
		}
	}
}

