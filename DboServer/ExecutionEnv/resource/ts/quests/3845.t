CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 24;
	rq = 0;
	tid = 3845;
	title = 384502;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 384507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
			CDboTSCheckClrQst
			{
				and = "3844;";
				flink = 0;
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
				conv = 384509;
				ctype = 1;
				idx = 5631101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 16781100;
				cnt0 = 1;
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
				cont = 384508;
				gtype = 2;
				area = 384501;
				goal = 384504;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 384505;
				stype = 256;
				taid = 1;
				title = 384502;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 384508;
			gtype = 2;
			oklnk = 2;
			area = 384501;
			goal = 384504;
			sort = 384505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 384502;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 384514;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 384501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
		}
	}
}

