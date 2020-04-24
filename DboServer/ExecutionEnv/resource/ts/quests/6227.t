CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 10;
	rq = 0;
	tid = 6227;
	title = 622702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 105;
			elnk = 255;
			nextlnk = 110;
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
		CDboTSContGCond
		{
			cid = 110;
			prelnk = "105;";
			nolnk = 255;
			rm = 1;
			yeslnk = 254;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 6227;
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
			prelnk = "110;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActObjConv
			{
				conv = 622709;
				idx = 21;
				widx = 500000;
				ctype = 1;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 3631100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 622708;
				gtype = 3;
				area = 622701;
				goal = 622704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 622705;
				stype = 2;
				taid = 1;
				title = 622702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 622714;
			nextlnk = 105;
			rwdexp = 0;
			rwdtbl = 622701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6431101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 622707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 38;
			}
			CDboTSCheckClrQst
			{
				and = "6226;";
				flink = 0;
				not = 0;
			}
			CDboTSClickObject
			{
				objidx = "21;";
				widx = 500000;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 622708;
			gtype = 3;
			oklnk = 2;
			area = 622701;
			goal = 622704;
			sort = 622705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 622702;
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
				itype = 0;
			}
		}
	}
}

