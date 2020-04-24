CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2441;
	title = 244102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 99098;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActItem
			{
				iidx0 = 99098;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99098;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 2;
				esctype = 0;
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
				conv = 244109;
				ctype = 1;
				idx = 2802103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 244101;
				cnt0 = 4;
				ectype = -1;
				etype = 0;
				qtidx = 244120;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 244108;
				m1fx = "-467.000000";
				m2fz = "564.000000";
				m1fy = "0.000000";
				sort = 244105;
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 244101;
				goal = 244104;
				m0fz = "1213.000000";
				m0widx = 14;
				m1ttip = 244115;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = 99098;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "832.000000";
				m2fx = "-411.000000";
				m2ttip = 244115;
				m0fx = "-424.000000";
				m0ttip = 244115;
				title = 244102;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			desc = 244114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 244101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2802103;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99098;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 244107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSCheckClrQst
			{
				and = "2440;";
				flink = 1;
				flinknextqid = "2442;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2802103;";
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
			cont = 244108;
			gtype = 2;
			oklnk = 2;
			area = 244101;
			goal = 244104;
			sort = 244105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 244102;
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

			CDboTSActItem
			{
				iidx0 = 99098;
				stype0 = 1;
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
			stdiag = 244107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

