CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6017;
	title = 601702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 6017;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
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
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 601714;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 601701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 627;
			}
			CDboTSClickNPC
			{
				npcidx = "3332301;";
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
				conv = 601709;
				ctype = 1;
				idx = 3332301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 601708;
				area = 601701;
				goal = 601704;
				m0fz = "-1648.068970";
				m0widx = 1;
				scitem = -1;
				sort = 601705;
				stype = 8;
				m0fy = "-67.476997";
				m0pat = 2;
				grade = -1;
				m0fx = "6829.956055";
				m0ttip = 601715;
				rwd = 100;
				taid = 1;
				title = 601702;
				gtype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 627;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 601707;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6016;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3332301;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 601708;
			gtype = 1;
			oklnk = 2;
			area = 601701;
			goal = 601704;
			sort = 601705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 601702;
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
			stdiag = 601707;
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
				iidx0 = 627;
				taid = 1;
				type = 1;
			}
		}
	}
}

