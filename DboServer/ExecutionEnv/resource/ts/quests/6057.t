CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6057;
	title = 605702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1215;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 100;
			prelnk = "7;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 605721;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1215;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 3;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 7;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 605709;
				ctype = 1;
				idx = 4371314;
				taid = 2;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1215;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 605708;
				area = 605701;
				goal = 605704;
				m0fz = "-2785.998047";
				m0widx = 1;
				scitem = -1;
				sort = 605705;
				stype = 8;
				m0fy = "0.000000";
				m0pat = 0;
				grade = -1;
				m0fx = "3101.422119";
				m0ttip = 605715;
				rwd = 100;
				taid = 1;
				title = 605702;
				gtype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 4211101;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = -1;
				taid = 3;
				widx0 = 1;
				dt0 = 605720;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 605708;
			gtype = 1;
			oklnk = 2;
			area = 605701;
			goal = 605704;
			sort = 605705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 605702;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "6;";
			nolnk = 5;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1215;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 605707;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4371314;";
			}
			CDboTSCheckClrQst
			{
				and = "6056;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 5;
			prelnk = "4;3;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 605714;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 605701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "8;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1215;
			}
			CDboTSClickNPC
			{
				npcidx = "4371314;";
			}
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
			stdiag = 0;
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
				iidx0 = 1215;
				taid = 1;
				type = 1;
			}
		}
	}
}

