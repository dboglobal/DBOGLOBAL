CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2447;
	title = 244702;

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
			stdiag = 244707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "4511513;";
			}
			CDboTSCheckClrQst
			{
				and = "2446;";
				flink = 1;
				flinknextqid = "2449;";
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
				conv = 244709;
				ctype = 1;
				idx = 4511513;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 6014107;
				cnt0 = 7;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 244708;
				m1fx = "173.000000";
				m1fy = "0.000000";
				sort = 244705;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1043.000000";
				grade = 132203;
				m0fx = "-38.000000";
				m0ttip = 244715;
				m1pat = 2;
				m1widx = 14;
				rwd = 100;
				taid = 1;
				title = 244702;
				gtype = 2;
				area = 244701;
				goal = 244704;
				m0fz = "1043.000000";
				m0widx = 14;
				m1ttip = 244715;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 244714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 244701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511513;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 244708;
			gtype = 2;
			oklnk = 2;
			area = 244701;
			goal = 244704;
			sort = 244705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 244702;
		}
	}
}

