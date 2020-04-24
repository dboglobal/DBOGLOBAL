CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2574;
	title = 257402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 102;
			elnk = 253;
			nextlnk = 254;
			prelnk = "101;";

			CDboTSActWPSFD
			{
				sid = 2574;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 257407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "4751312;";
			}
			CDboTSCheckClrQst
			{
				and = "2572;";
				flink = 1;
				flinknextqid = "2578;";
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
				conv = 257409;
				ctype = 1;
				idx = 4751312;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1238;
				cnt0 = 10;
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
				cont = 257408;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 257405;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "-1924.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 257401;
				goal = 257404;
				m0fz = "-3500.000000";
				m0widx = 1;
				m1ttip = 257415;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-3680.000000";
				m3ttip = 257415;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3640.000000";
				m2fx = "-1857.000000";
				m2ttip = 257415;
				m0fx = "-1872.000000";
				m0ttip = 257415;
				m3pat = 2;
				title = 257402;
				m1fx = "-1712.000000";
				m2fz = "-3829.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 257414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 257401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751312;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "102;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "102;1;0;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 257408;
			gtype = 2;
			oklnk = 2;
			area = 257401;
			goal = 257404;
			sort = 257405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 257402;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 257407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1238;
				taid = 1;
				type = 1;
			}
		}
	}
}

