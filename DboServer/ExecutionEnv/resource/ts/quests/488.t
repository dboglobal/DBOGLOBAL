CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 488;
	title = 48802;

	CNtlTSGroup
	{
		gid = 254;

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

			CDboTSActItem
			{
				iidx0 = 99029;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 48818;
			}
			CDboTSActItem
			{
				iidx0 = 99029;
				stype0 = 1;
				taid = 2;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 5;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99029;
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
			stdiag = 48807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "4192101;";
			}
			CDboTSCheckClrQst
			{
				and = "481;";
				flink = 1;
				flinknextqid = "496;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 48814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 48801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4192101;";
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
				conv = 48809;
				ctype = 1;
				idx = 4192101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 48808;
				gtype = 2;
				area = 48801;
				goal = 48804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 48805;
				stype = 2;
				taid = 1;
				title = 48802;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 489;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 48818;
				taid = 3;
				widx0 = 1;
				dt0 = 48819;
				esctype = 0;
				otype0 = 7;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 48808;
			gtype = 2;
			oklnk = 2;
			area = 48801;
			goal = 48804;
			sort = 48805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 48802;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
	}
}

