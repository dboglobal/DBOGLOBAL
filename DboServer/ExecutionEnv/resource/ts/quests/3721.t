CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3721;
	title = 372102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372107;
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
				npcidx = "7072201;";
			}
			CDboTSCheckClrQst
			{
				and = "3720;";
				flink = 0;
				flinknextqid = "3722;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372109;
				ctype = 1;
				idx = 7072201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 19841100;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 19843201;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 372108;
				m1fx = "-3111.889893";
				m2fz = "-5116.100098";
				m1fy = "-61.259998";
				sort = 372105;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 372101;
				goal = 372104;
				m0fz = "-5531.709961";
				m0widx = 15;
				m1ttip = 372115;
				m2fy = "-83.040001";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "-85.709999";
				m0pat = 2;
				m1fz = "-4849.129883";
				m2fx = "-2908.209961";
				m2ttip = 372116;
				m0fx = "-2873.770020";
				m0ttip = 372115;
				title = 372102;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372108;
			gtype = 1;
			oklnk = 2;
			area = 372101;
			goal = 372104;
			sort = 372105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372102;
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
			desc = 372114;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7071112;";
			}
		}
	}
}

