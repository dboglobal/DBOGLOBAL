CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1442;
	title = 144202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 144209;
				ctype = 1;
				idx = 3143113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 144208;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 144205;
				m2widx = 2;
				grade = 132203;
				m1pat = 0;
				m1widx = 2;
				m3fx = "4206.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				area = 144201;
				goal = 144204;
				m0fz = "1091.890015";
				m0widx = 2;
				m1ttip = 144216;
				m2fy = "0.000000";
				m2pat = 0;
				m3fz = "4388.000000";
				m3ttip = 144226;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "1221.119995";
				m2fx = "-530.679993";
				m2ttip = 144217;
				m0fx = "-599.780029";
				m0ttip = 144215;
				m3pat = 0;
				title = 144202;
				m1fx = "-499.540009";
				m2fz = "1034.969971";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 3;
				idx0 = 1500;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 1501;
				taid = 3;
				esctype = 0;
				idx2 = 1502;
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
				itype = 3;
			}
		}
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
				eitype = 3;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 144208;
			gtype = 3;
			oklnk = 2;
			area = 144201;
			goal = 144204;
			sort = 144205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 144202;
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
			desc = 144214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 144201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1500;
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1502;
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1501;
			}
			CDboTSClickNPC
			{
				npcidx = "3143113;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 144207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "3143113;";
			}
			CDboTSCheckClrQst
			{
				and = "1431;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 1;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 1;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 1500;
				iidx2 = 1502;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 1501;
				iprob0 = "1.000000";
			}
		}
	}
}

