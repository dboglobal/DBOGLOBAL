CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2911;
	title = 291102;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1565;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActNPCConv
			{
				conv = 291109;
				ctype = 1;
				idx = 4481203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 291108;
				m1fx = "2562.510010";
				m1fy = "0.000000";
				sort = 291105;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2109.090088";
				grade = 132203;
				m0fx = "2512.439941";
				m0ttip = 291115;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 291102;
				gtype = 2;
				area = 291101;
				goal = 291104;
				m0fz = "-2122.219971";
				m0widx = 1;
				m1ttip = 291116;
				stype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 291114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 291101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4481203;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 291107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSCheckClrQst
			{
				and = "2905;2910;";
				flink = 1;
				flinknextqid = "2912;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4481203;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 291108;
			gtype = 2;
			oklnk = 2;
			area = 291101;
			goal = 291104;
			sort = 291105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 291102;
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
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
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
			stdiag = 291107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
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
				iidx0 = 1565;
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
	}
}

