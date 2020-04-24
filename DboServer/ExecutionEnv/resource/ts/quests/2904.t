CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2904;
	title = 290402;

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
			stdiag = 290407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
			CDboTSCheckClrQst
			{
				and = "2902;";
				flink = 1;
				flinknextqid = "2905;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 290409;
				ctype = 1;
				idx = 4511312;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1512;
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
				cont = 290408;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 290405;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2213.149902";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 290401;
				goal = 290404;
				m0fz = "-2370.520020";
				m0widx = 1;
				m1ttip = 290415;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-2600.290039";
				m3ttip = 290415;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2360.530029";
				m2fx = "2275.550049";
				m2ttip = 290415;
				m0fx = "2548.250000";
				m0ttip = 290415;
				m3pat = 2;
				title = 290402;
				m1fx = "2335.219971";
				m2fz = "-2454.270020";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 290414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 290401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 290408;
			gtype = 2;
			oklnk = 2;
			area = 290401;
			goal = 290404;
			sort = 290405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 290402;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx0 = 1512;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 290407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

