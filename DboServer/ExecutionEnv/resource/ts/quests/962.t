CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 962;
	title = 96202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			stdiag = 96207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 34;
			}
			CDboTSClickNPC
			{
				npcidx = "4072106;";
			}
			CDboTSCheckClrQst
			{
				and = "965;";
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
				conv = 96209;
				ctype = 1;
				idx = 4072106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 96208;
				gtype = 1;
				area = 96201;
				goal = 96204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 96205;
				stype = 2;
				taid = 1;
				title = 96202;
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
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92025;
				eitype = 6;
				tidx0 = 590;
				ectype = 0;
				iidx0 = -1;
				dt0 = 96221;
				esctype = 0;
				otype0 = 7;
				etype = 0;
				it0 = 96220;
				taid = 1;
				widx0 = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 96214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 96201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072106;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 96208;
			gtype = 1;
			oklnk = 2;
			area = 96201;
			goal = 96204;
			sort = 96205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 96202;
		}
	}
}

