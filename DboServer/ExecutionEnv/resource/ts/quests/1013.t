CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1013;
	title = 101302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 598;
				ectype = -1;
				etype = 0;
				iidx0 = 787;
				it0 = 101330;
				taid = 1;
				widx0 = 1;
				dt0 = 101331;
				esctype = 0;
				otype0 = 7;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 101318;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 252;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 1013;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "6;2;";
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
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 101307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 37;
			}
			CDboTSClickNPC
			{
				npcidx = "5061301;";
			}
			CDboTSCheckClrQst
			{
				and = "1012;";
				flink = 1;
				not = 0;
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
				conv = 101309;
				ctype = 1;
				idx = 5061301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 101308;
				gtype = 1;
				area = 101301;
				goal = 101304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 101305;
				stype = 2;
				taid = 1;
				title = 101302;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 1013;
				start = 0;
				taid = 2;
				uof = 0;
			}
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
			desc = 101314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 101301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061301;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 101308;
			gtype = 1;
			oklnk = 2;
			area = 101301;
			goal = 101304;
			sort = 101305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 101302;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 101307;
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
				iidx0 = 787;
				taid = 1;
				type = 1;
			}
		}
	}
}

