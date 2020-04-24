CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 843;
	title = 84302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 84309;
				ctype = 1;
				idx = 1653106;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 4;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 84320;
				taid = 3;
				widx0 = 3;
				dt0 = 84321;
				esctype = 0;
				otype0 = 7;
			}
			CDboTSActRegQInfo
			{
				cont = 84308;
				gtype = 2;
				area = 84301;
				goal = 84304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 84305;
				stype = 2;
				taid = 1;
				title = 84302;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 84314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 84301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 84308;
			gtype = 2;
			oklnk = 2;
			area = 84301;
			goal = 84304;
			sort = 84305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 84302;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 84307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "1653106;";
			}
			CDboTSCheckClrQst
			{
				and = "842;";
				flink = 1;
				flinknextqid = "868;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
	}
}

