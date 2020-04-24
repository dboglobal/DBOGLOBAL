CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6007;
	title = 600702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 253;
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
			cid = 254;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 600707;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6006;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2804128;";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 600714;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 600701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 10;
				iidx = 621;
			}
			CDboTSClickNPC
			{
				npcidx = "2804128;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 600709;
				ctype = 1;
				idx = 2804128;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 600708;
				area = 600701;
				goal = 600704;
				m0fz = "-2786.083008";
				m0widx = 1;
				scitem = -1;
				sort = 600705;
				stype = 8;
				m0fy = "-45.289001";
				m0pat = 2;
				grade = -1;
				m0fx = "6411.785156";
				m0ttip = 600715;
				rwd = 100;
				taid = 1;
				title = 600702;
				gtype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 621;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 600708;
			gtype = 1;
			oklnk = 2;
			area = 600701;
			goal = 600704;
			sort = 600705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 600702;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 600707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 621;
				taid = 1;
				type = 1;
			}
		}
	}
}

